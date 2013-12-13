





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
    
    template<typename TableList, typename SelectedColumnsTuple>
    class cross_expression : public deferred_range<typename detail::row_type<TableList, SelectedColumnsTuple>::type>
    {
            using base = deferred_range<typename detail::row_type<TableList, SelectedColumnsTuple>::type>;
            using table_type = typename TableList::template at<TableList::size-1>::type;
        public: 
        using value_type = typename base::value_type;
        using implied_select = typename detail::row_type<TableList, SelectedColumnsTuple>::modified_tuple;
    
        cross_expression(std::shared_ptr<pqxx::connection> & connection, std::string query_without_select)
            : base(connection), _query_without_select(query_without_select) 
        {
            _query_without_select += " CROSS JOIN " + metaspace::meta_table<table_type>::name();
        }
        template<typename OtherTable>
        auto cross_join() -> cross_expression<typename TableList::template push_back<OtherTable>::type, SelectedColumnsTuple >
        {
            return { this->_connection, _query_without_select };
        }
        auto limit(std::size_t count) -> cross_expression<TableList, SelectedColumnsTuple> &
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
            return this->execute(implied_select(), _query_without_select, TableList());
        }
        private:
        std::string  _query_without_select;
    };
    

}} //tagsql # development
