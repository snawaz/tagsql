





#pragma once


#include <type_traits>
#include <string>
#include <memory>
#include <array>

#include <foam/meta/seq.h++>
#include <foam/meta/typelist.h++>

#include <tagsql/development/table.h++>
#include <tagsql/development/meta_table.h++>
#include <tagsql/development/meta_column.h++>
#include <tagsql/development/tags.h++>
#include <tagsql/development/deferred_range.h++>


#include <pqxx/pqxx>


namespace tagsql { namespace development
{
	template<typename Bucket>
	struct join_expression;

    template<typename Bucket>
    class on_expression : public deferred_range<Bucket>
    {
            using base = deferred_range<Bucket>;
    
		public:

            on_expression(std::shared_ptr<pqxx::connection> & connection, std::string query)
            : base(connection, query) 
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
