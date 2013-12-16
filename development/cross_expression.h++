





#pragma once


#include <type_traits>
#include <string>
#include <memory>
#include <array>

#include <tagsql/development/table.h++>
#include <tagsql/development/meta_table.h++>
#include <tagsql/development/meta_column.h++>
#include <tagsql/development/tags.h++>
#include <tagsql/development/deferred_range.h++>
#include <tagsql/development/row_type_helper.h++>
#include <pqxx/pqxx>


namespace tagsql { namespace development
{
	template<typename Bucket>
	class join_expression;

    template<typename Bucket> 
    class cross_expression : public deferred_range<Bucket>
    {
            using base = deferred_range<Bucket>;
			using table_type = typename Bucket::join_tables::template at<Bucket::join_tables::size-1>::type;
        public: 
        	cross_expression(std::shared_ptr<pqxx::connection> & connection, std::string query_without_select)
            	: base(connection, query_without_select + " CROSS JOIN " + metaspace::meta_table<table_type>::name())
			{
        	}
			
			template<typename OtherTable>
            auto cross_join(OtherTable) -> cross_expression<typename Bucket::template add_join<OtherTable, join_type::cross>::type>
            {
                return {this->_connection, this->_query_without_select};
            }
			
			template<typename OtherTable>
            auto join(OtherTable) -> join_expression<typename Bucket::template add_join<OtherTable, join_type::inner>::type>
            {
                return { this->_connection, this->_query_without_select };
            }
            
			template<typename OtherTable>
            auto left_join(OtherTable) -> join_expression<typename Bucket::template add_join<OtherTable, join_type::left>::type>
            {
                return { this->_connection, this->_query_without_select };
            }
            
			template<typename OtherTable>
            auto right_join(OtherTable) -> join_expression<typename Bucket::template add_join<OtherTable, join_type::right>::type>
            {
                return { this->_connection, this->_query_without_select };
            }
	
			template<typename OtherTable>
            auto full_join(OtherTable) -> join_expression<typename Bucket::template add_join<OtherTable, join_type::full>::type>
            {
                return { this->_connection, this->_query_without_select };
            }
    };
    

}} //tagsql # development
