

#pragma once

#include <tuple>
#include <cstdlib>
#include <ctime>
#include <functional>

#include <foam/strlib/format.h>
#include <foam/strlib/strlib.h>

#include "platform.h++"
#include "metadata.h++"
#include "code_writer.h++"
#include "config_reader.h++"
#include "string_algo.h++"

namespace tagsql 
{
    struct syntax_error : public std::runtime_error
    {
	using std::runtime_error::runtime_error;
    };

    class code_generator
    {
	public:
	    code_generator(tagsql::meta::metadata & meta, meta::config_reader & config, meta::type_mapper & mapper) 
		: _meta(meta), _config(config), _mapper(mapper)
	    {
	    }
	    void generate()
	    {
		std::string outdir = _config["outdir"];
		if ( outdir.back() != '/' ) 
		    outdir += '/';

		ensure_dir(outdir);

		auto add_project_ns_tokens = [=](std::string item) mutable 
		{ 
		    auto  copy = this->project_ns_tokens(); 
		    copy.push_back(std::move(item)); 
		    return copy; 
		}; 

		std::map<std::string, std::tuple<code_writer, std::vector<std::string>>> writers;
		writers.insert(std::make_pair("table", std::make_tuple(code_writer(outdir + "table.h++"), add_project_ns_tokens("schema"))));
		writers.insert(std::make_pair("tags", std::make_tuple(code_writer(outdir + "tags.h++"), project_ns_tokens())));
		writers.insert(std::make_pair("tags_impl", std::make_tuple(code_writer(outdir + "tags_impl.h++"), project_ns_tokens())));
		writers.insert(std::make_pair("keys", std::make_tuple(code_writer(outdir + "keys.h++"), project_ns_tokens())));
		writers.insert(std::make_pair("meta_table", std::make_tuple(code_writer(outdir + "meta_table.h++"), add_project_ns_tokens("metaspace"))));
		writers.insert(std::make_pair("formatter", std::make_tuple(code_writer(outdir + "formatter.h++"), add_project_ns_tokens("formatting"))));

		auto stream = [&](std::string key) -> code_writer& { return std::get<0>(writers.at(key)); };

		for(auto & item : writers)
		{
		    std::get<0>(item.second).pragma("once");
		    std::get<0>(item.second).open_ns(std::get<1>(item.second));
		}

		try
		{
		    generate_schema(stream("table"));
		    generate_tags(stream("tags"));
		    generate_tags_impl(stream("tags_impl"));
		    generate_keys(stream("keys"));
		    generate_meta_table(stream("meta_table"));
		    generate_formatter(stream("formatter"));

		    auto && template_files = get_templates();
		    for(auto const & template_file : template_files)
		    {
			auto outfile = template_file.substr(0, template_file.size() - 10) + "h++"; 
			generate_from_template(outdir + outfile, "templates/" + template_file);
		    }
		}
		catch(...)
		{
		    std::cout << "no" << std::endl;
		    throw;
		}

	    }

	private: 
	
	    std::string project_ns() 
	    { 
		return _config["namespace"]; 
	    }

	    std::vector<std::string> project_ns_tokens()
	    { 
		static auto tokens = split(project_ns(), "::"); 
		return tokens; 
	    }

	    auto get_templates() -> std::vector<std::string>
	    {
		auto filter = [](std::string const & filename) { return ::foam::strlib::endswith(".template++", filename); };
		return read_directory("templates", filter);
	    }

	    void generate_schema(code_writer & out)
	    {
		out.include(join("/", project_ns_tokens()) + "/column.h++");
		out.include(join("/", project_ns_tokens()) + "/column.h++");
		out.include(join("/", project_ns_tokens()) + "/tags.h++");

		auto generate_schema_for = [&](meta::meta_table const & table) 
		{
		    static int tag_id = 0;
		    ++tag_id;

		    namespace fs = ::foam::strlib;

		    out.newline()
		       .comment("This definition of schema for table '" + table.name + "'.")
		       .writeln(fs::format("namespace txzi{0} = ::{1}::{2}_tag;", tag_id, project_ns(), table.name))
		       .begin_class(table.name, true);

		    for(auto const & c : table.columns )
		    {
			auto mapped = _mapper[c.type];
			auto var_type = fs::format("column <txzi{0}::{1}_t>", tag_id, c.name);
			auto nullability = c.is_nullable? "-" : "non-nullable";
			auto constraint = c.is_primary_key? "primary_key" : c.is_foreign_key ? "foreign_key" : "-";
			auto server_default = c.has_server_default? "server_default" : "-";
			auto comment = fs::format("{0}\t{1}\t{2}\t{3}", mapped.type, nullability, constraint, server_default);
			out.declare_variable(var_type, c.name, mapped.header, comment); 
		    }
		    out.end_class();
		};

		for (auto const & table : _meta.meta_tables() ) 
		{
		    std::cout <<"Generating C++ table for database table '" << table.name << "' ... ";
		    generate_schema_for(table);
		    std::cout <<"ok" << std::endl; 
		}
	
		out.include("foam/strlib/format.h");
		namespace fs = ::foam::strlib;
		for (auto const & table : _meta.meta_tables() ) 
		{
		    std::cout <<"Generating C++ table for database table '" << table.name << "' ... ";
		    std::vector<int> indices(table.columns.size());
		    std::iota(indices.begin(), indices.end(), 0);
		    auto mem = [](meta::meta_column const & c) { return "item." + c.name; };
		    auto ind = [](int i) { return ::foam::strlib::format("{{{0}}}", i); };
		    out.newline(1)
		       .begin_block(fs::format("std::ostream& operator<<(std::ostream& out, {0} const & item)", table.name))
		       .writeln(fs::format("return ::foam::strlib::print(out, \"{0}\", {1});", join(",", transform(indices, ind)), join(",", transform(table.columns, mem))))
		       .end_block();
		    std::cout <<"ok" << std::endl; 
		}
	    }
	    void generate_tags(code_writer & out)
	    {
		out.include(join("/", project_ns_tokens()) + "/tags_impl.h++");

		auto generate_tags_for = [&](meta::meta_table const & table) 
		{
		    namespace fs = ::foam::strlib;

		    out.newline();

		    std::srand(static_cast<unsigned int>(std::time(0)));
		    auto random_columns = [&](std::size_t count) {
			std::vector<meta::meta_column> ret;
			for(auto const & c : table.columns)
			    if ( ret.size() <= count && std::rand() % 2 == 0 ) ret.push_back(c);
			if (ret.size() == 0 )
			    ret.push_back(table.columns[0]);
			return ret;
		    };

		    auto xselect = [&](meta::meta_column const & c) { return fs::format("{0}t::{1}", table.name[0], c.name); };
		    auto select = join(", ", transform(random_columns(4), xselect));

		    auto xwhere = [&](meta::meta_column const & c) { return fs::format("{0}t::{1} = {1}_value", table.name[0], c.name); };
		    auto where = join("&& ", transform(random_columns(2), xwhere));

		    out.mcomment(fs::format("The definition of tags for table '{0}.'", table.name),
			    	 "The user can use these tags in select-clause, where-clause, and on-clause, as well as when accessing values from select results.",
				 "",
				 "Examples,",
				 "",
				 fs::format("\tusing namespace = {0}::schema; //bring all the table names from 'schema' namespace.", project_ns()),
				 fs::format("\tnamespace {2}t = {0}::{1}_tag; //make easy-to-use alias for specific 'tag' namespace.", project_ns(), table.name, table.name[0]),
				 "",
				 fs::format("\tauto items = context.select( {0} )", select),
				 fs::format("\t                    .from<{0}>()", table.name),
				 fs::format("\t                    .where()", where),
				 "",
				 "Hope that helps. :-)");

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
	    void generate_keys(code_writer & out)
	    {
		out.include(join("/", project_ns_tokens()) + "/tags_impl.h++");

		auto generate_keys_for = [&](meta::meta_table const & table)
		{
		    namespace fs = ::foam::strlib;

		    out.newline()
		       .open_ns(table.name + "_tag");

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
		       .writeln(fs::format("using all = std::tuple<{0}>;", join(",", transform(table.columns, [](meta::meta_column const & c) { return c.name + "_t"; }))))
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
		out.include(join("/", project_ns_tokens()) + "/meta_column.h++");
		out.open_ns("schema");
		for (auto const & table : _meta.meta_tables() ) 
		    out.writeln("struct " + table.name + ";");
		out.close_ns();

		auto generate_tags_impl_for = [&](meta::meta_table const & table)
		{
		    namespace fs = ::foam::strlib;

		    out.newline();
		    out.open_ns(table.name + "_tag");

		    auto bool_ = [](bool b) { return b ? "true" : "false"; };
		    for(auto const & c : table.columns)
		    {
			auto mapped = _mapper[c.type];
			auto base = fs::format("metaspace::meta_column_t<schema::{0},{1},{2},{3}>", table.name, mapped.type, bool_(c.is_nullable), bool_(c.has_server_default));
			out.mcomment(fs::format("column_name    = {0}", c.name),
				     fs::format("column_type    = {0} <--- {1}", mapped.type, c.type),
				     fs::format("table_name     = {0}", table.name),
				     fs::format("primary_key    = {0}", bool_(c.is_primary_key)),
				     fs::format("foreign_key    = {0}", bool_(c.is_foreign_key)),
				     fs::format("nullable       = {0}", bool_(c.is_nullable)),
				     fs::format("server_default = {0}", bool_(c.has_server_default)))
			    .begin_class(fs::format("{0}_t : {1}", c.name, base), true)
			    .writeln(fs::format("using base = {0};", base))
			    .newline()
			    .writeln(fs::format("{0} {1};", mapped.type, c.name))
			    .newline()
			    .writeln(fs::format("{0}_t() : base(\"{0}\"){{}}", c.name))
			    .newline()
			    .writeln(fs::format("void set({0} value) {{ {1} = std::move(value); }}", mapped.type, c.name))
			    .newline()
			    .writeln(fs::format("{0} const & get() const {{ return {1}; }}", mapped.type, c.name))
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
		out.include(join("/", project_ns_tokens()) + "/meta_table_base.h++");
		out.include(join("/", project_ns_tokens()) + "/keys.h++");

		auto generate_meta_table_for = [&](meta::meta_table const & table)
		{
		    namespace fs = ::foam::strlib;

		    out.newline()
		       .writeln("template<>")
		       .begin_class(fs::format("meta_table<{0}::schema::{1}> : public meta_table_base<{0}::schema::{1}>", project_ns(), table.name), true)
		       .writeln(fs::format("using  sequence_type = ::foam::meta::genseq_t<{0}>;", table.columns.size()))
		       .writeln(fs::format("using  primary_key   = {0}::{1}_tag::primary_key;", project_ns(), table.name))
		       .writeln(fs::format("using  foreign_keys  = {0}::{1}_tag::foreign_keys;", project_ns(), table.name))
		       .writeln(fs::format("using  columns_tuple = {0}::{1}_tag::all;", project_ns(), table.name))
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
		auto generate_formatter_for = [&](meta::meta_table const & table)
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
	    void generate_from_template(std::string outfile, std::string template_file)
	    {
		namespace fs = ::foam::strlib;

		fs::print("Generating C++ {0} from {1}", outfile, template_file);
		
		auto __BEGIN_HEADER__    = "__BEGIN_HEADER__";
		auto __END_HEADER__      = "__END_HEADER__";
		auto __BEGIN_NAMESPACE__ = "__BEGIN_NAMESPACE__";
		auto __END_NAMESPACE__   = "__END_NAMESPACE__";
		auto __NAMESPACE__       = "__NAMESPACE__";

		code_writer out { outfile };
		std::ifstream file { template_file };
	
		std::string const space(4, ' ');

		char const *mode = nullptr;
		for (std::string line; std::getline(file, line); )
		{
		    if ( line.find(__BEGIN_HEADER__ ) != std::string::npos )
		    {
			if ( mode != nullptr )
			    throw syntax_error(fs::format("__BEGIN_HEADER__ must not appear without closing previous {0} first. template = {1}", mode, template_file));
			mode = __BEGIN_HEADER__;
		    }
		    else if ( line.find(__BEGIN_NAMESPACE__) != std::string::npos )
		    {
			if ( mode != nullptr )
			    throw syntax_error(fs::format("__BEGIN_NAMESPACE__ must not appear without closing previous {0} first. template = {1}", mode, template_file));
			mode = __BEGIN_NAMESPACE__;
			auto tokens = split(line, "(");
			auto ns = project_ns_tokens();
			if (tokens.size() != 1)
			{
			    auto more = split(tokens[1].substr(0, tokens[1].size() -1), "::");
			    ns.insert(ns.end(), more.begin(), more.end());
			}
			out.open_ns(ns);
		    }
		    else if ( line.find(__END_HEADER__) != std::string::npos )
		    {
		   	if ( mode != __BEGIN_HEADER__ )
			    throw syntax_error(fs::format("__END_HEADER__ must be preceded by __BEGIN_HEADER__. template = {0}", template_file));
			mode = nullptr;
		    }
		    else if ( line.find(__END_NAMESPACE__) != std::string::npos )
		    {
		   	if ( mode != __BEGIN_NAMESPACE__ )
			    throw syntax_error(fs::format("__END_NAMESPACE__ must be preceded by __BEGIN_NAMESPACE__. template = {0}", template_file));
			mode = nullptr;
			out.close_ns();
		    }
		    else if ( mode == __BEGIN_HEADER__ )
			out.writeln(replace(replace(line, __NAMESPACE__, join("/", project_ns_tokens())), "\t", space));
			//out.writeln(replace(line, __NAMESPACE__, join("/", project_ns_tokens())));
		    else if ( mode == __BEGIN_NAMESPACE__ )
			out.writeln(replace(replace(line, __NAMESPACE__, project_ns()), "\t", space));
			//out.writeln(replace(line, __NAMESPACE__, project_ns()));
		    else
			//out.writeln(line);
			out.writeln(replace(line, "\t", space));
		    auto i = line.find("_query_without");
		    if ( i != std::string::npos  && template_file.find("from_exp") != std::string::npos)
			std::cout << line << "(" << i << ")\n";
		}
		if ( mode != nullptr )
		    throw syntax_error(fs::format("The template file {0} is not close by proper macro. It is still opened by {1}.", template_file, mode));
		fs::print("ok\n");
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
	    meta::metadata      & _meta;
	    meta::config_reader & _config;
	    meta::type_mapper   & _mapper;
    };
}
