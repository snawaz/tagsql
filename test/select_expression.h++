





#pragma once


#include <memory>

#include <tagsql/test/from_expression.h++>

#include <pqxx/pqxx>


namespace tagsql { namespace test
{
	
	template<typename SelectedColumnsTuple>
	class select_expression 
	{
	    public: 
	
		select_expression(std::shared_ptr<pqxx::connection> & connection)
		    : _connection(connection) {}
	
		template<typename Table>
		auto from() -> from_expression<Table,SelectedColumnsTuple>
		{
		    return { _connection };
		}
	    private:
		std::shared_ptr<pqxx::connection> _connection;
	};
	

}} //tagsql # test
