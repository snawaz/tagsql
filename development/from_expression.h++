





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
#include <tagsql/development/common_clauses.h++>

#include <pqxx/pqxx>


namespace tagsql { namespace development
{
    template<typename Bucket>
    class from_expression : public deferred_range<Bucket>, public clause_picker<Bucket>
    {
            using base = deferred_range<Bucket>;
			using select_tags = typename Bucket::select;
			using columns_tuple = typename metaspace::meta_table<typename Bucket::from>::columns_tuple;
        public: 
            using value_type = typename base::value_type; 
            using implied_select = typename std::conditional<metaspace::meta_table<value_type>::is_table, columns_tuple, select_tags>::type;
    
            from_expression(std::shared_ptr<pqxx::connection> & connection)
            : base(connection, " FROM " + metaspace::meta_table<typename Bucket::from>::name() ) { }

			/*
            template<typename OtherTable>
            auto cross_join() -> cross_expression<typename Bucket::template add_join<OtherTable, join_type::cross>::type>
            {
                return {this->_connection, _query_without_select};
            }
			*/

            template<typename OtherTable>
            auto inner_join(OtherTable) -> join_expression<typename Bucket::template add_join<OtherTable, join_type::inner>::type>
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

		#if 0
			template<typename Condition>
            auto where(Condition const & expr) -> where_expression<typename Bucket::template add_where<Condition>::type>
            {
				static_assert(is_condition_expression<Condition>::value, 
						"Constraint Violation : expression passed to where() is invalid. It must be a condition expression involving column(s).");
				static_assert(expression_traits<Condition>::tables::template exists<typename Bucket::from>::value, 
						"Constraint Violation : table mismatch for the condition passed to from_expression::where().");

                this->_query_without_select += " WHERE " + expr.repr();
				return { this->_connection, this->_query_without_select };
            }
		#endif
    };
    

}} //tagsql # development
