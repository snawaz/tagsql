





#pragma once


#include <type_traits>
#include <string>
#include <memory>
#include <array>

#include <foam/strlib/format.h>
#include <foam/meta/seq.h++>
#include <tagsql/test/table.h++>
#include <tagsql/test/meta_table.h++>
#include <tagsql/test/meta_column.h++>
#include <tagsql/test/tags.h++>

#include <pqxx/pqxx>


namespace tagsql { namespace test
{
	
	template<typename Table>
	class update_expression
	{
	    public: 
	
		update_expression(std::shared_ptr<pqxx::connection> const & connection, std::string query)
		    : _connection(connection), _query(std::move(query)), _executed(false) {}
	
		auto where(condition clause) -> std::string
		{
		    _query += " WHERE " + clause._value;
		    return execute();
		}
		auto all() -> std::string
		{
		    return execute();
		}
	    private:
		auto execute() -> std::string
		{
		    if ( _executed )
			return _query;
		    _executed = true;
		    pqxx::work transaction { *_connection } ;
		    auto result = transaction.exec(_query);
		    transaction.commit();
		    return _query;
		}
	    private:
		std::shared_ptr<pqxx::connection>   _connection;
		std::string                       _query;
		bool                              _executed;
	};
	

}} //tagsql # test
