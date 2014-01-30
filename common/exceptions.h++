

#pragma once

#include <string>
#include <stdexcept>

namespace tagsql
{
	struct syntax_error : public std::runtime_error
	{
		using std::runtime_error::runtime_error;
	};
	
	struct unsupported_type : public std::runtime_error
	{
		using std::runtime_error::runtime_error;
	};

	struct parse_exception : std::runtime_error
	{
		using std::runtime_error::runtime_error;
	};
	
	struct constraint_violation : public std::runtime_error
	{
		using std::runtime_error::runtime_error;
	};

	struct os_error : public std::runtime_error
	{
		using std::runtime_error::runtime_error;
	};
    
	class database_error : public std::exception
	{
		public:
			database_error(std::string query, std::exception const & server_error) 
				: _query(query), _server_error(server_error.what())
			{}
			virtual char const* what() const noexcept override
			{
				static const auto all = std::string("query:\n")
					+ "---\n"
					+ _query + "\n"
					+ "---\n"
					+ "error at server follows as:\n" + _server_error;
				return all.c_str();
			}
			std::string const & query() const { return _query; }
			std::string const & server_error() const { return _server_error; }
		private:
			std::string _query;
			std::string _server_error;
	};

}
