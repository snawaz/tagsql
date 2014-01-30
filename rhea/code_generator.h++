

#pragma once

#include <tuple>
#include <cstdlib>
#include <ctime>
#include <functional>

#include <foam/strlib/format.h>
#include <foam/strlib/strlib.h>

#include <tagsql/common/exceptions.h++>
#include <tagsql/common/string_algo.h++>

#include <tagsql/rhea/platform.h++>
#include <tagsql/rhea/metadata.h++>
#include <tagsql/rhea/code_writer.h++>
#include <tagsql/rhea/config_reader.h++>
#include <tagsql/rhea/supported_types.h++>

namespace tagsql { namespace rhea
{
	class code_generator
	{
		public:
			code_generator(metadata & meta, config_reader & config, type_mapper & mapper) 
				: _meta(meta), _config(config), _mapper(mapper) { }
			
			void generate()
			{
				namespace fs = ::foam::strlib;

				std::string source_path  = _config["source_path"];
				std::string include_path = _config["include_path"];
				if ( ! fs::endswith(include_path, source_path) )
					throw syntax_error(fs::format("in the config, the include_path '{0}' must be a suffix of source_path '{1}'", include_path, source_path));

				auto outdir = std::move(source_path);
				if ( outdir.back() != '/' ) 
					outdir += '/';

				outdir += _config["dbspace"];
				if ( outdir.back() != '/' ) 
					outdir += '/';

				try
				{
					fs::print("ensuring '{0}' exists (create if it doesn't) ... ", outdir);
					ensure_dir(outdir);
					fs::println("ok");
				}
				catch(...)
				{
					fs::println("no");
					throw;
				}

				auto add_project_ns_tokens = [=](std::string item) mutable 
				{ 
					auto  copy = this->project_ns_tokens(); 
					copy.push_back(std::move(item)); 
					return copy; 
				}; 
				auto add_tagsql_ns_tokens = [=](std::string item) mutable 
				{ 
					return std::vector<std::string>{"tagsql", item};
				}; 

				std::map<std::string, std::tuple<code_writer, std::vector<std::string>>> writers;
			
				//few friendly lambdas!
				auto add_writer = [&](std::string name, std::string filename, std::vector<std::string> ns)
				{
					writers.insert(std::make_pair(std::move(name), std::make_tuple(code_writer(std::move(filename)),std::move(ns))));
				};
				auto stream = [&](std::string key) -> code_writer& { return std::get<0>(writers.at(key)); };

				//add various code writers for various codefile to be generated
				add_writer("table", outdir + "table.h++", add_project_ns_tokens("schema"));
				add_writer("tags", outdir + "tags.h++", project_ns_tokens());
				add_writer("tags_impl", outdir + "tags_impl.h++", project_ns_tokens());
				add_writer("keys", outdir + "keys.h++", project_ns_tokens());
				add_writer("meta_table", outdir + "meta_table.h++", add_tagsql_ns_tokens("metaspace"));
				add_writer("universal_tags", outdir + "universal_tags.h++", add_project_ns_tokens("universal_tags"));
				add_writer("__easy_include__", outdir + _config["dbspace"] + ".h++", {});
				//add_writer("formatter", outdir + "formatter.h++", add_project_ns_tokens("formatting"));

				//add #pragma once in all header files and the initial namespace
				for(auto & item : writers)
				{
					std::get<0>(item.second).pragma("once");
					std::get<0>(item.second).open_ns(std::get<1>(item.second));
				}

				try
				{
					generate_keys(stream("keys"));
					generate_meta_table(stream("meta_table"));
					generate_schema(stream("table"));
					generate_tags(stream("tags"));
					generate_tags_impl(stream("tags_impl"));
					generate_universal_tags(stream("universal_tags"));
					//generate_formatter(stream("formatter"));

					auto & out = stream("__easy_include__");
					out.include(tagsql_include().append("data_context.h++"));
					out.newline();
					for(auto const & i : writers)
					{
						if (i.first != "__easy_include__")
							out.include(project_include().append(i.first + ".h++"));
					}
				}
				catch(...)
				{
					fs::println("no");
					throw;
				}
			}

		private: 

			std::string project_ns() 
			{
				return _config["namespace"] + "::" + _config["dbspace"] ; 
			}
			std::vector<std::string> project_ns_tokens()
			{
				static auto tokens = split(project_ns(), "::");
				return tokens;
			}
			struct include_t
			{
				std::string data;
				auto append(std::string file) -> include_t
				{
					data += "/" + file;
					return std::move(*this);
				}
				operator std::string() const
				{
					return data; 
				}
			};

			include_t _project_include;
			include_t project_include()
			{
				if (_project_include.data.empty())
				{
					auto path = _config["include_path"]; 
					if ( path.back() != '/' )
						path += "/";
					path += _config["dbspace"];
					_project_include.data = std::move(path);
				}
				return _project_include;
			}

			include_t _tagsql_include {"tagsql"};
			include_t tagsql_include()
			{
				return _tagsql_include;
			}

			void generate_schema(code_writer & out)
			{
				out.include(tagsql_include().append("named_tuple.h++"));
				out.include(project_include().append("tags.h++"));
				
				auto generate_schema_for = [&](meta_table const & table)
				{
					static int tag_id = 0;
					++tag_id;
					
					namespace fs = ::foam::strlib;
					
					out.newline()
					   .comment("This definition of schema for table '" + table.name + "'.")
					   .writeln(fs::format("namespace txzi{0} = ::{1}::{2}_tag;", tag_id, project_ns(), table.name));
					out.write(fs::format("using _{0}_base_t = ::tagsql::named_tuple<", table.name));
					std::string spaces = out.spaces_();
					auto const & columns = table.columns;
					auto widen = [](std::string const & s, std::size_t w) 
					{
						if ( s.size() < w ) 
							return s + std::string(w - s.size(), ' ');
						else 
							return s;
					};
					for(std::size_t i = 0; i < columns.size(); ++i)
					{
						auto const & c = columns[i];
						auto mapped = _mapper[c.type];
						auto nullability = c.is_nullable? "-" : "non-nullable";
						auto constraint = c.is_primary_key? "primary_key" : c.is_foreign_key ? "foreign_key" : "-";
						auto server_default = c.has_server_default? "server_default" : "-";
						auto comment = fs::format("{0}\t{1}\t{2}\t{3}", widen(mapped.type, 20), widen(nullability, 12), widen(constraint,12), widen(server_default,14));
						if ( i == columns.size() - 1)
							out.write("\n" + spaces + widen(fs::format("txzi{0}::{1}_t", tag_id, c.name), 25), 0, comment);
						else
							out.write("\n" + spaces + widen(fs::format("txzi{0}::{1}_t,", tag_id, c.name), 25), 0, comment);
					}
					out.writeln(fs::format("\n{0}>;", spaces.substr(0, spaces.size()-2)))
					   .begin_class(fs::format("{0}_t : public _{0}_base_t", table.name), true)
					   .writeln(fs::format("using  _{0}_base_t::_{0}_base_t;", table.name))
					   .end_class();
				};

				for (auto const & table : _meta.meta_tables() )
				{
					std::cout <<"Generating C++ table for database table '" << table.name << "' ... ";
					generate_schema_for(table);
					std::cout <<"ok" << std::endl; 
				}
			}
			void generate_tags(code_writer & out)
			{
				out.include(project_include().append("tags_impl.h++"));
				
				auto generate_tags_for = [&](meta_table const & table)
				{
					namespace fs = ::foam::strlib;

					out.newline();
					
					out.open_ns(table.name + "_tag");
					for ( auto const & c : table.columns) 
					{
						auto mapped = _mapper[c.type];
						auto nullability = c.is_nullable? "-" : "non-nullable";
						auto constraint = c.is_primary_key? "primary_key" : c.is_foreign_key ? "foreign_key" : "-";
						auto server_default = c.has_server_default? "server_default" : "-";
						auto comment = fs::format("{0}\t{1}\t{2}\t{3}", mapped.type, nullability, constraint, server_default);
						out.declare_variable(fs::format("static const {0}_t", c.name), c.name, mapped.header, comment); 
					}
					out.close_ns();
				};

				for (auto const & table : _meta.meta_tables() )
				{
					std::cout <<"Generating C++ tags for database table '" << table.name << "' ... ";
					generate_tags_for(table);
					std::cout <<"ok" << std::endl;
				}
			}

			struct universal_tag_info
			{
				std::vector<std::string> tables;
				bool                     is_column;
				bool                     is_table;
			};
			void generate_universal_tags(code_writer & out)
			{
				out.include(project_include().append("table.h++"));
				out.include(project_include().append("tags_impl.h++"));
				out.include(tagsql_include().append("core/tiny_types.h++"));
				
				namespace fs = ::foam::strlib;

				std::map<std::string, universal_tag_info> tag_map;
				for (auto const & table : _meta.meta_tables() ) 
				{
					tag_map[table.name].is_table = true;
					for(auto const & column : table.columns )
					{
						auto & tag_info = tag_map[column.name];
						tag_info.is_column = true;
						tag_info.tables.push_back(table.name);
					}
				}

				out.open_ns("detail");
				for(auto const p : tag_map)
				{
					auto const & tag_info = p.second;
					if ( tag_info.is_column )
					{
						out.comment(fs::format("get meta-functions for {0}", p.first))
						   .writeln("template<typename Table>")
						   .writeln(fs::format("struct get_{0};", p.first))
						   .newline();

						for(auto const table : tag_info.tables )
						{
							out.writeln("template<>")
							   .writeln(fs::format("struct get_{0}<schema::{1}_t> {{ using type = {1}_tag::{0}_t; }};", p.first, table))
							   .newline();
						}
					}
				}
				out.close_ns();
				
				for(auto const p : tag_map)
				{
					auto const & tag_info = p.second;
					out.newline()
					   .begin_class(p.first + "_t", true, "static const")
					   .writeln(fs::format("using _is_column = std::{0}_type;", tag_info.is_column ? "true" : "false"))
					   .writeln(fs::format("using _is_table  = std::{0}_type;", tag_info.is_table ? "true" : "false"))
					   .writeln(fs::format("using _is_unique = {0};", tag_info.is_table ? "::tagsql::null" : (tag_info.tables.size() == 1? "std::true_type" : "std::false_type")))
					   .newline();
					if ( tag_info.is_column )
					{
						out.writeln(fs::format("using tables = ::foam::meta::typelist<{0}>;", join(",", transform(tag_info.tables, [](std::string s) { return "schema::" + s + "_t"; }))))
					   	   .newline()
					       .writeln("template<typename Table>")
					       .writeln(fs::format("struct get_column : detail::get_{0}<Table>{{}};", p.first))
					       .newline();
					}
					if ( tag_info.is_table )
					{
						out.writeln(fs::format("using table = schema::{0}_t;", p.first))
					       .newline();

						auto const & mts = _meta.meta_tables();
						auto const & it = std::find_if(mts.begin(), mts.end(), [&](meta_table const & mt) { return mt.name == p.first; });
						if ( it == mts.end() )
							throw std::logic_error(fs::format("No table found with name '{0}'", p.first));
						for(auto const & column : it->columns)
						{
							out.declare_variable(fs::format("{0}_tag::{1}_t", p.first, column.name), column.name);
						}
						out.newline();

					}
					out.end_class(p.first);
				}

			}
			void generate_keys(code_writer & out)
			{
				out.include(project_include().append("tags_impl.h++"));

				auto generate_keys_for = [&](meta_table const & table)
				{
					namespace fs = ::foam::strlib;

					out.newline().open_ns(table.name + "_tag");

					std::string pkey = "null";
					std::vector<std::string> fkeys;
					for(auto const & c : table.columns)
					{
						auto tag = c.name + "_t";
						if (c.is_primary_key)
							pkey = tag;
						if (c.is_foreign_key)
							fkeys.push_back(fs::format("std::tuple<{0},{1}>", tag, fs::format("{0}_tag::{1}_t", c.ref_table_name, c.ref_column_name))); 
					}

					out.writeln(fs::format("using primary_key  = {0};",pkey), "assuming at most one column can be primary_key.")
					   .newline()
					   .writeln(fs::format("using foreign_keys = std::tuple<{0}>;", join(",", fkeys)))
					   .newline()
					   .writeln(fs::format("using all = std::tuple<{0}>;", join(",", transform(table.columns, [](meta_column const & c) { return c.name + "_t"; }))))
					   .newline()
					   .writeln("template<typename Tag>")
					   .writeln(fs::format("std::string qualify(Tag const & tag) {{ return \"{0}.\" + tag.column_name; }}", table.name));
					out.close_ns();
				};
				for (auto const & table : _meta.meta_tables() ) 
				{
					std::cout <<"Generating C++ keys for database table '" << table.name << "' ... ";
					generate_keys_for(table);
					std::cout <<"ok" << std::endl; 
				}
			}
			
			void generate_tags_impl(code_writer & out)
			{
				out.include(tagsql_include().append("core/meta_column.h++"));
				out.include(tagsql_include().append("support/supported_types.h++"));

				out.open_ns("schema");
				for (auto const & table : _meta.meta_tables() ) 
					out.writeln("struct " + table.name + "_t;");
				out.close_ns();
				auto generate_tags_impl_for = [&](meta_table const & table)
				{
					namespace fs = ::foam::strlib;

					out.newline();
					out.open_ns(table.name + "_tag");
					auto bool_ = [](bool b) { return b ? "true" : "false"; };
					for(auto const & c : table.columns)
					{
						auto mapped = _mapper[c.type];
						auto base = fs::format("::tagsql::metaspace::meta_column_t<schema::{0}_t,{1},{2},{3}>", table.name, mapped.type, bool_(c.is_nullable), bool_(c.has_server_default));
						auto ti = get_type_info(c.type);
						auto size = c.character_max_length + ti.size; //assumption : one of the operand is expected to be zero (untested)
						auto sql_data_type = size
												? fs::format("::tagsql::support::types::multi<::tagsql::support::types::{0},{1}>", ti.name, size)
												: fs::format("::tagsql::support::types::single<::tagsql::support::types::{0}>", ti.name);

						out.mcomment(fs::format("column_name    = {0}", c.name),
										size 
											? fs::format("column_type    = {0} <--- {1} [{2}]", mapped.type, c.type, size)
											: fs::format("column_type    = {0} <--- {1}", mapped.type, c.type),
										fs::format("table_name     = {0}", table.name),
										fs::format("primary_key    = {0}", bool_(c.is_primary_key)),
										fs::format("foreign_key    = {0}", bool_(c.is_foreign_key)),
										fs::format("nullable       = {0}", bool_(c.is_nullable)),
										fs::format("server_default = {0}", bool_(c.has_server_default)))
							.begin_class(fs::format("{0}_t : {1}", c.name, base), true)
							.writeln(fs::format("using base = {0};", base))
							.writeln(fs::format("using sql_data_type = {0};", sql_data_type))
							.newline()
							.writeln("template<typename T>")
							.begin_class("named_member", true)
							.writeln(fs::format("T {0};", c.name))
							.end_class()
							.newline()
							.writeln(fs::format("{0}_t() : base(\"{0}\"){{}}", c.name))
							.newline()
							.writeln(fs::format("static char const* type_name() {{ return \"{0}\"; }}", mapped.type))
							.end_class()
							.newline();
						out.include(mapped.header);
					}
					out.close_ns();
				};
				
				for (auto const & table : _meta.meta_tables() ) 
				{
					std::cout <<"Generating C++ tags_impl for database table '" << table.name << "' ... ";
					generate_tags_impl_for(table);
					std::cout <<"ok" << std::endl; 
				}
			}
			
			void generate_meta_table(code_writer & out)
			{
				out.include(tagsql_include().append("core/meta_table_base.h++"));
				out.include(project_include().append("keys.h++"));
				out.include(project_include().append("table.h++"));
				
				auto generate_meta_table_for = [&](meta_table const & table)
				{
					namespace fs = ::foam::strlib;

					out.newline()
					   .writeln("template<>")
					   .begin_class(fs::format("meta_table<::{0}::schema::{1}_t> : public meta_table_base<::{0}::schema::{1}_t>", project_ns(), table.name), true)
					   .writeln(fs::format("using  sequence_type = ::foam::meta::genseq_t<{0}>;", table.columns.size()))
					   .writeln(fs::format("using  primary_key   = ::{0}::{1}_tag::primary_key;", project_ns(), table.name))
					   .writeln(fs::format("using  foreign_keys  = ::{0}::{1}_tag::foreign_keys;", project_ns(), table.name))
					   .writeln(fs::format("using  columns_tuple = ::{0}::{1}_tag::all;", project_ns(), table.name))
					   .writeln(fs::format("static std::string name() {{  return \"{0}\"; }}", table.name))
					   .writeln("static sequence_type index_sequence() { return {}; }")
					   .writeln(fs::format("static constexpr std::size_t size() {{ return {0}; }}",table.columns.size()))
					   .writeln("static columns_tuple columns() { return {}; }")
					   .end_class();
				};

				for (auto const & table : _meta.meta_tables() )
				{
					std::cout <<"Generating C++ meta_table for database table '" << table.name << "' ... ";
					generate_meta_table_for(table);
					std::cout <<"ok" << std::endl;
				}
			}
			void generate_formatter(code_writer & out)
			{
				auto generate_formatter_for = [&](meta_table const & table)
				{
					namespace fs = ::foam::strlib;

					out.newline()
					   .writeln("template<typename Adaptor>")
					   .begin_class(fs::format("formatter<::{0}::schema::{1}, Adaptor>", project_ns(), table.name))
					   .access_specifier(accessibility::Public)
					   .writeln(fs::format("using value_type = ::{0}::schema::{1};", project_ns(), table.name))
					   .writeln("using output_type = typename Adaptor::value_type;")
					   .newline()
					   .begin_function("formatter(value_type const & value)");

					for(auto const & c : table.columns)
						out.writeln(fs::format("_adaptor.write(value.{0}, _output);", c.name));

					out.end_function()
					   .writeln("output_type const & output() const { return _output; }")
					   .access_specifier(accessibility::Private)
					   .declare_variable("Adaptor", "_adaptor")
					   .declare_variable("output_type", "_output")
					   .end_class();
				};
				out.include(join("/", project_ns_tokens()) + "/insert_adaptor.h++");

				for (auto const & table : _meta.meta_tables() )
				{
					std::cout <<"Generating C++ tags_impl for database table '" << table.name << "' ... ";
					generate_formatter_for(table);
					std::cout <<"ok" << std::endl;
				}
			}
			static std::string& replace(std::string & s, std::string const & oldstr, std::string const & newstr)
			{
				auto i = s.find(oldstr);
				if ( i != std::string::npos ) 
					return replace(s.replace(i, oldstr.size(), newstr), oldstr, newstr);
				return s;
			}
			static std::string replace(std::string && s, std::string const & oldstr, std::string const & newstr)
			{
				return std::move(replace(s, oldstr, newstr)); //call the other overload, then move the result, because it is temporary!
			}
		private:
			metadata      & _meta;
			config_reader & _config;
			type_mapper   & _mapper;
	};

}} // tagsql # rhea
