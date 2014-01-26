

#pragma once

#include <tagsql/development/clauses/join_clause.h++>
#include <tagsql/development/deferred_range.h++>
#include <tagsql/development/schema/meta_table.h++>

#include <string>
#include <pqxx/pqxx>

namespace tagsql { namespace development
{
    template<typename SelectQuery>
    class composite_table : public deferred_range<SelectQuery>
    {
            using base = deferred_range<SelectQuery>;
        public: 
    
			composite_table(std::shared_ptr<pqxx::connection> & connection, std::string query)
            : base(connection, query) 
            { 
            }

            template<typename OtherTable>
            auto cross_join(OtherTable) -> composite_table<typename SelectQuery::template add_join<OtherTable, join_type::cross>::type>
            {
                return {this->_connection, this->_query_without_select + " CROSS JOIN " + metaspace::meta_table<OtherTable>::name() };
            }

            template<typename OtherTable>
            auto join(OtherTable) -> join_clause<typename SelectQuery::template add_join<OtherTable, join_type::inner>::type>
            {
                return { this->_connection, this->_query_without_select };
            }

            template<typename OtherTable>
            auto left_join(OtherTable) -> join_clause<typename SelectQuery::template add_join<OtherTable, join_type::left>::type>
            {
                return { this->_connection, this->_query_without_select };
            }
            
			template<typename OtherTable>
            auto right_join(OtherTable) -> join_clause<typename SelectQuery::template add_join<OtherTable, join_type::right>::type>
            {
                return { this->_connection, this->_query_without_select };
            }
	
			template<typename OtherTable>
            auto full_join(OtherTable) -> join_clause<typename SelectQuery::template add_join<OtherTable, join_type::full>::type>
            {
                return { this->_connection, this->_query_without_select };
            }
    };
    

}} //tagsql # development
