





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
#include <tagsql/development/cross_expression.h++>
#include <tagsql/development/join_expression.h++>
#include <tagsql/development/where_expression.h++>
#include <tagsql/development/deferred_range.h++>

#include <pqxx/pqxx>


namespace tagsql { namespace development
{
    template<typename Bucket>
    class from_expression : public deferred_range<Bucket>
    {
            using base = deferred_range<Bucket>;
			using select_tags = typename Bucket::select;
			using columns_tuple = typename metaspace::meta_table<typename Bucket::from>::columns_tuple;
        public: 
            using value_type = typename base::value_type; 
            using implied_select = typename std::conditional<metaspace::meta_table<value_type>::is_table, columns_tuple, select_tags>::type;
    
            from_expression(std::shared_ptr<pqxx::connection> & connection)
            : base(connection, "FROM " + metaspace::meta_table<typename Bucket::from>::name() ) { }

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
