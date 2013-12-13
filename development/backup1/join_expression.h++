





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
#include <pqxx/pqxx>


namespace tagsql { namespace development
{
    template<sql_join_type JoinType, typename TableList, typename SelectedColumnsTuple>
    //class join_expression : public deferred_range<typename detail::row_type<TableList, SelectedColumnsTuple>::type>
    class join_expression : public deferred_range<join_expression<JoinType, TableList, SelectedColumnsTuple>>
    {
            //using base = deferred_range<typename detail::row_type<TableList, SelectedColumnsTuple>::type>;
    		using base =  deferred_range<join_expression<JoinType, TableList, SelectedColumnsTuple>>;
            using table_type = typename TableList::template at<TableList::size-1>::type;
        public: 
            using value_type = typename base::value_type;
            //using value_type = typename detail::row_type<TableList, SelectedColumnsTuple>::type;
            using implied_select = typename detail::row_type<TableList, SelectedColumnsTuple>::modified_tuple;
    
        	join_expression(std::shared_ptr<pqxx::connection> & connection, std::string query_without_select)
            	: base(connection), _query_without_select(query_without_select) 
        	{
            	if ( JoinType == sql_join_type::inner) _query_without_select += " INNER JOIN " + metaspace::meta_table<table_type>::name();
            	else if ( JoinType == sql_join_type::left) _query_without_select += " LEFT JOIN " + metaspace::meta_table<table_type>::name();
            	else if ( JoinType == sql_join_type::right) _query_without_select += " RIGHT JOIN " + metaspace::meta_table<table_type>::name();
        	}
        	template<typename OtherTable>
        	auto join() -> join_expression<sql_join_type::inner, typename TableList::template push_back<OtherTable>::type , SelectedColumnsTuple>
        	{
            	return {this->_connection};
        	}
			template<typename ConditionExpression>
            auto on(ConditionExpression const & expr) -> join_expression & //<Table, SelectedColumnsTuple> &
            {
				static_assert(is_condition_expression<ConditionExpression>::value, 
						"Constraint Violation : expression passed to where() is invalid. It must be a condition expression involving column(s).");
				static_assert(expression_traits<ConditionExpression>::tables::template exists<table_type>::value, 
						"Constraint Violation : table mismatch for the condition passed to from_expression::where().");

                _query_without_select += " ON " + expr.repr();
                return *this;
            }
        	auto limit(std::size_t count)
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
				//TableList x = 10;
            	return this->execute(implied_select(), _query_without_select, TableList());
        	}
        private:
        	std::string                       _query_without_select;
    };
    
}} //tagsql # development
