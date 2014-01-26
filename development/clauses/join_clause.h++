

#pragma once

#include <tagsql/development/meta_table.h++>

#include <string>
#include <pqxx/pqxx>

namespace tagsql { namespace development
{
    template<typename SelectQuery>
    class composite_table;

	template<typename SelectQuery>
	class join_clause 
	{
			using table_type = typename SelectQuery::join_tables::template at<SelectQuery::join_tables::size-1>::type;
		public:
    
        	join_clause(std::shared_ptr<pqxx::connection> & connection, std::string query_without_select)
            	: _connection(connection), _query_without_select(query_without_select)
        	{
				static const std::string token = SelectQuery::joins::template at<SelectQuery::joins::size-1>::type::join_type::token;
				_query_without_select +=  " " + token + "  " + metaspace::meta_table<table_type>::name();
        	}
			
			template<typename Condition>
            auto on(Condition const & expr) -> composite_table<typename SelectQuery::template add_on<Condition>::type>
            {
				static_assert(is_condition_expression<Condition>::value, 
						"Invalid Query : expression passed to on() is invalid. It must be a condition expression involving column(s).");
				static_assert(expression_traits<Condition>::tables::template exists<table_type>::value, 
						"Invalid Query : table mismatch for the condition passed to on().");

                return { this->_connection, this->_query_without_select + " ON " + expr.repr() };
            }
		private:
			std::shared_ptr<pqxx::connection> _connection;
			std::string                       _query_without_select;
    };
    
}} //tagsql # development
