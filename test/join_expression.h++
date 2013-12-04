





#pragma once


#include <type_traits>
#include <string>
#include <memory>
#include <array>

#include <tagsql/developement/table.h++>
#include <tagsql/developement/meta_table.h++>
#include <tagsql/developement/meta_column.h++>
#include <tagsql/developement/tags.h++>
#include <tagsql/developement/deferred_range.h++>
#include <tagsql/developement/row_type_helper.h++>
#include <pqxx/pqxx>


namespace tagsql { namespace developement
{
    
    enum class sql_join_type
    {
        inner,
        left,
        right,
    };
    
    template<sql_join_type JoinType, typename TableList, typename SelectedColumnsTuple>
    class join_expression : public deferred_range<typename detail::row_type<TableList, SelectedColumnsTuple>::type>
    {
            using base = deferred_range<typename detail::row_type<TableList, SelectedColumnsTuple>::type>;
            using table_type = typename TableList::template at<TableList::size-1>::type;
        public: 
            using value_type = typename base::value_type;
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
        auto on(condition clause) 
        {
            _query_without_select += " ON " + clause._value;
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
        protected:
        virtual std::vector<value_type>& deferred_exec()
        {
            return this->execute(implied_select(), _query_without_select);
        }
        private:
        std::string                       _query_without_select;
    };
    

}} //tagsql # developement
