

#pragma once

#include <string>
#include <stdexcept>

namespace tagsql
{
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

} //tagsql
