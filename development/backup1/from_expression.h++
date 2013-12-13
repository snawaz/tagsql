





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
#include <tagsql/development/deferred_range.h++>


#include <pqxx/pqxx>


namespace tagsql { namespace development
{
    
    template<typename Table, typename SelectedColumnsTuple>
    //class from_expression : public deferred_range<typename metaspace::meta_table<Table>::template row_type<SelectedColumnsTuple>::type> 
    class from_expression : public deferred_range<from_expression<Table,SelectedColumnsTuple>>
    {
            //using base = deferred_range<typename metaspace::meta_table<Table>::template row_type<SelectedColumnsTuple>::type>;
            using base = deferred_range<from_expression<Table,SelectedColumnsTuple>>;
        public: 
            using value_type = typename base::value_type; 
            //using value_type = typename metaspace::meta_table<Table>::template row_type<SelectedColumnsTuple>::type;
            using implied_select = typename std::conditional<metaspace::meta_table<value_type>::is_table, 
				  														typename metaspace::meta_table<Table>::columns_tuple, SelectedColumnsTuple>::type;
    
            from_expression(std::shared_ptr<pqxx::connection> & connection)
            : base(connection) 
            { 
                _query_without_select =  " FROM " + metaspace::meta_table<Table>::name();
            }
            template<typename OtherTable>
            auto cross_join() -> cross_expression<foam::meta::typelist<Table, OtherTable>, SelectedColumnsTuple>
            {
                return {this->_connection, _query_without_select};
            }
            template<typename OtherTable>
            auto join(OtherTable) -> join_expression<sql_join_type::inner, foam::meta::typelist<Table, OtherTable>, SelectedColumnsTuple>
            {
                return {this->_connection, _query_without_select};
            }
			template<typename ConditionExpression>
            auto where(ConditionExpression const & expr) -> from_expression<Table, SelectedColumnsTuple> &
            {
				static_assert(is_condition_expression<ConditionExpression>::value, 
						"Constraint Violation : expression passed to where() is invalid. It must be a condition expression involving column(s).");
				static_assert(expression_traits<ConditionExpression>::tables::template exists<Table>::value, 
						"Constraint Violation : table mismatch for the condition passed to from_expression::where().");

                _query_without_select += " WHERE " + expr.repr();
                return *this;
            }
            auto limit(std::size_t count) -> from_expression<Table, SelectedColumnsTuple> &
            {
                _query_without_select += " LIMIT " + std::to_string(count);
                return *this;
            }
            auto exec() -> std::vector<value_type>
            {
                return deferred_exec(); 
            }
            std::vector<value_type>& deferred_exec()
            {
                return this->execute(implied_select(), _query_without_select, ::foam::meta::typelist<Table>());
            }
        private:
            std::string  _query_without_select;
    };
    

}} //tagsql # development
