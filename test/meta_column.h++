





#pragma once


#include <type_traits>
#include <string>
#include <array>

#include <tagsql/developement/condition.h++>

#include <tagsql/string_algo.h++>


namespace tagsql { namespace developement { namespace metaspace
{
    
    template<typename Table>
    struct meta_table;
    
    template<typename Table, typename ColumnType>
    struct update_data
    {
        using column_type = ColumnType;
    
        std::string const column_name;
        column_type const value;
    };
    
    template<typename Table, typename ColumnType, bool Nullable, bool ServerDefault>
    struct meta_column
    {
        using table_type  = Table;
        using column_type = ColumnType;
    
        static const bool is_nullable = Nullable;
        static const bool server_default = ServerDefault;
    
        std::string const column_name;
    
        meta_column(std::string column_name) : column_name(std::move(column_name)) {}
    
        auto operator()(column_type const & value) const -> update_data<Table, ColumnType>
        {
        return {column_name, value};
        }
    
        condition operator < (column_type value) const
        {
        return {column_name + " < " + std::to_string(value)};
        }
    
        condition operator <= (column_type value) const
        {
        return {column_name + " <= " + std::to_string(value)};
        }
    
        condition operator == (column_type value) const
        {
        return {column_name + " = " + std::to_string(value)};
        }
    
        condition operator >= (column_type value) const
        {
        return {column_name + " >= " + std::to_string(value)};
        }
    
        condition operator > (column_type value) const
        {
        return {column_name + " > " + std::to_string(value)};
        }
    
        condition operator == (std::nullptr_t) const
        {
        return {column_name + " IS NULL" };
        }
    
        condition operator != (std::nullptr_t) const
        {
        return {column_name + " IS NOT NULL" };
        }
    
        template<typename Table0, typename ColumnType0, bool Nullable0, bool ServerDefault0>
        condition operator == (meta_column<Table0, ColumnType0, Nullable0, ServerDefault0> const & value) const
        {
        return {meta_table<Table>::name() + "." + column_name + " = " + meta_table<Table0>::name() + "." + value.column_name};
        }
    };
    
    template<typename Table, typename ColumnType, bool Nullable, bool ServerDefault>
    struct meta_column_special: meta_column<Table, ColumnType, Nullable, ServerDefault>
    {
        using base = meta_column<Table, ColumnType, Nullable, ServerDefault>;
    
        using base::base;
    
        condition like(std::string pattern) const
        {
            return {base::column_name + " LIKE '" + pattern + "'"};
        }
    
        template<typename ... String>
        condition like_any(String ... patterns) const
        {
            //http://stackoverflow.com/questions/4928054/postgresql-wildcard-like-for-any-of-a-list-of-words
            std::array<std::string, sizeof...(patterns)> v{("'" + std::string(patterns) + "'")...};
            return {base::column_name + " LIKE ANY (array[" + join(", ", v) + "])"};
        }
    
        condition not_like(std::string pattern) const
        {
        return {base::column_name + " NOT LIKE '" + pattern + "'"};
        }
        condition ilike(std::string pattern) const
        {
        return {base::column_name + " ILIKE '" + pattern + "'"};
        }
        condition not_ilike(std::string pattern) const
        {
        return {base::column_name + " NOT ILIKE '" + pattern + "'"};
        }
        condition similar(std::string pattern) const
        {
        return {base::column_name + " SIMILAR TO '" + pattern + "'"};
        }
        condition not_similar(std::string pattern) const
        {
        return {base::column_name + " NOT SIMILAR TO '" + pattern + "'"};
        }
    };
    
    template<typename Table, typename ColumnType, bool Nullable, bool ServerDefault>
    using meta_column_t = typename std::conditional<std::is_fundamental<ColumnType>::value, meta_column<Table,ColumnType,Nullable,ServerDefault>,
                                meta_column_special<Table,ColumnType,Nullable,ServerDefault>>::type;
    
    

}}} //tagsql # developement # metaspace
