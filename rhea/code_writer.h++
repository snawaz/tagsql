
#pragma once

#include <fstream>
#include <sstream>
#include <vector>
#include <array>
#include <stack>
#include <string>
#include <set>
#include <map>
#include <memory>
#include <stdexcept>
#include <cassert>

#include <tagsql/common/string_algo.h++>

namespace tagsql
{
	enum class accessibility
	{
		Private,
		Protected,
		Public,
	};

	static const std::map<accessibility, std::string> access_names 
	{
		{ accessibility::Private, "private" },
		{ accessibility::Protected, "protected" },
		{ accessibility::Public, "public" }
	};
	
	class code_writer_exception : public ::std::runtime_error
	{
		using ::std::runtime_error::runtime_error;
	};
   
	template<typename Item>
	std::ostream& operator<<(std::ostream& out, std::vector<Item> const & items)
	{
		for(auto const & item : items)
			out << item << ",";
		return out;
	}

	class code_writer
	{
		public:
			explicit code_writer(std::string filename)
				: _filename(std::move(filename)),
				  _fs(new std::ofstream(_filename)),
				  _ss(new std::ostringstream),
				  _accessibility_on(false)
			{
				if ( !*_fs ) 
					throw code_writer_exception("Couldn't open file for writing : " + _filename);
			}
			~code_writer()
			{
				if ( _fs ) //check moved or not!
				{
					for(std::size_t i = 0, size = _namespaces.size(); i < size; ++i)
						close_ns();
					*_fs << "\n\n";
					for(auto const & header : _headers)
						*_fs << "#include <" << header << ">\n";
					*_fs << "\n\n" << _ss->str();
				}
			}
	    
			code_writer(code_writer &&) = default;
			code_writer& operator=(code_writer &&) = default;

			auto pragma(std::string const & text="once") -> code_writer &
			{
				*_fs << "\n\n#pragma " << text << "\n";
				return *this;
			}
			auto comment(std::string const & text) -> code_writer &
			{
				writeln("//" + text);
				return *this;
			}
			template<typename ... String>
			auto mcomment(String && ... text) -> code_writer &
			{
				std::array<std::string,sizeof...(String)> lines { text ... };
				writeln("/*");
				for(auto const & line : lines)
					writeln("* + " + line);
				writeln("*/");
				return *this;
			}
			auto open_ns(std::string const & ns, int newlines=0) -> code_writer &
			{
				std::vector<std::string> nss { ns };
				return open_ns(nss, newlines);
			}
			auto open_ns(std::vector<std::string> const & namespaces, int newlines=0) -> code_writer &
			{
				if ( namespaces.empty() ) 
					return *this;
				newline(newlines);
				auto f = [](std::string const & s) { return "namespace " + s; };
				writeln(join(" { ", transform(namespaces, f)));
				writeln("{");
				++_level;
				_namespaces.push(namespaces);
				return *this;
			}
			auto close_ns() -> code_writer &
			{
				--_level;
				assert(_level >= 0 );
				newline();
				auto const & ns = _namespaces.top();
				writeln(std::string(ns.size(), '}') + " //" + join(" # ", ns));
				_namespaces.pop();
				return *this;
			}
			auto begin_class(std::string name, bool use_struct=false, std::string qualifier="") -> code_writer &
			{
				if (qualifier.empty())
					writeln((use_struct ? "struct " : "class ") + name);
				else
					writeln(qualifier + " " +(use_struct ? "struct " : "class ") + name);
				writeln("{");
				++_level;
				return *this;
			}
			auto end_class(std::string object_name = "", std::string initializer="") -> code_writer &
			{
				_level -= _accessibility_on ? 2 : 1;
				assert(_level >= 0 );
				_accessibility_on = false;
				if ( object_name.empty() )
					writeln("};");
				else
					writeln("}" + object_name + "{" + initializer + "};"); 
				return *this;
			}
			auto declare_variable(std::string type, std::string var, std::string header="", std::string comment="", int typew = 45, int varw = 25) -> code_writer &
			{
				auto widen = [](std::string const & s, std::size_t w) 
				{
					if ( s.size() < w ) 
						return s + std::string(w - s.size(), ' ');
					else 
						return s;
				};
				auto text = widen(type, typew) + " "  + widen(var + ";", varw);
				if ( comment.empty() )
					writeln(text);
				else
					writeln(widen(text, typew + varw) + "//" + comment);
				include(header);
				return *this;
	    	}
			auto declare_class(std::string class_type, std::string class_name, std::string header="", std::string comment="", int classw=7, int namew=25) -> code_writer &
			{
				return declare_variable(class_type, class_name, header, comment, classw, namew);
			}
			auto include(std::string header) -> code_writer &
			{
				if ( header.empty() ) 
					return *this;
				_headers.insert(std::move(header));
				return *this;
			}
			auto newline(std::size_t count=1) -> code_writer &
			{
				*_ss << std::string(count, '\n');
				return *this;
			}
			auto begin_function(std::string signature) -> code_writer &
			{
				return begin_block(std::move(signature));
	    	}
			auto end_function() -> code_writer &
			{
				return end_block();
			}
	    	auto begin_block(std::string text) -> code_writer &
			{
				writeln(text);
				writeln("{");
				++_level;
				return *this;
			}
			auto end_block() -> code_writer &
			{
				--_level;
				assert(_level >= 0 );
				writeln("}");
				return *this;
	    	}
	    	auto access_specifier(accessibility value) -> code_writer &
	    	{
				if ( _accessibility_on )
				{
					--_level;
					assert(_level >= 0 );
					writeln(access_names.at(value) + ":");
					++_level;
				}
				else
				{
					writeln(access_names.at(value) + ":");
					_accessibility_on = true;
					++_level;
				}
				return *this;
			}
			auto spaces() -> std::string
			{
				return std::string(_level, '\t');
	    	}
			auto spaces_() -> std::string
			{
				//return std::string(_column_pos, ' ');
				
				if ( _column_pos )
					return std::string(_level, '\t') + std::string(_column_pos, ' ');
				else
					return std::string(_level, '\t');
				
			}
	    	auto write(std::string text, std::size_t newlines = 0, std::string comment = "") -> code_writer &
	    	{
				if ( comment.empty() ) 
					text = (_column_pos ? "" : spaces()) + text;
				else
					text = (_column_pos ? "" : spaces()) + text + " //" + comment;
				*_ss << text;
				_column_pos = text.size();
				if ( newlines )
				{
					*_ss << std::string(newlines, '\n');
					_column_pos = 0;
				}
				return *this;
	    	}
			auto writeln(std::string text,  std::string comment = "") -> code_writer &
			{
				_column_pos = 0;
				return write(text, 1, comment);
			}
			auto writels(std::vector<std::string> lines, std::string comment = "") -> code_writer &
			{
				_column_pos = 0;
				for(auto const & line : lines)
					write(line, 1, comment);
				return *this;
			}
		private:
			std::string                           _filename;
			std::unique_ptr<std::ofstream>        _fs;
			std::unique_ptr<std::ostringstream>   _ss;
			int                                   _level = 0;
			std::set<std::string>                 _headers;
			std::stack<std::vector<std::string>>  _namespaces;
			bool                                  _accessibility_on;
			std::size_t                           _column_pos = 0;
    };
}
