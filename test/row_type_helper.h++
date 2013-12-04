





#pragma once


#include <type_traits>

#include <tagsql/developement/meta_table.h++>
#include <tagsql/developement/meta_column.h++>
#include <foam/meta/typelist.h++>


namespace tagsql { namespace developement { namespace detail
{
    
    template<typename T>
    using bare_type_t = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
    
    //column_types
    template<typename ColumnsTuple>
    struct column_types;
    
    template<typename ...Columns>
    struct column_types<std::tuple<Columns...>> { using type = std::tuple<typename Columns::column_type...>; };
    
    //all_columns_of_all_tables
    template<typename TableList>
    struct all_columns_of_all_tables;
    
    template<typename ... Tables>
    struct all_columns_of_all_tables<::foam::meta::typelist<Tables...>> 
    {
        using type = bare_type_t<decltype(std::tuple_cat(typename column_types<typename metaspace::meta_table<Tables>::columns_tuple>::type() ... ))>;
        using modified_tuple = bare_type_t<decltype(std::tuple_cat(typename metaspace::meta_table<Tables>::columns_tuple() ... ))>;
    };
    template<typename Table>
    struct all_columns_of_all_tables<::foam::meta::typelist<Table>> 
    {
        using type = Table; 
        using modified_tuple = typename metaspace::meta_table<Table>::columns_tuple;
    };
    
    //table or selected columns
    template<typename TableList, typename SelectedColumnsTuple>
    struct table_or_selected_columns;
    
    template<typename Table, typename ... Rest, typename SelectedColumnsTuple>
    struct table_or_selected_columns<::foam::meta::typelist<Table, Rest...>, SelectedColumnsTuple>
    {
        using possible_type = typename metaspace::meta_table<Table>::template row_type<SelectedColumnsTuple>::type;
        using type = typename std::conditional<std::is_same<possible_type, Table>::value, 
                                                  Table, 
                                                  typename table_or_selected_columns<::foam::meta::typelist<Rest...>, SelectedColumnsTuple>::type>::type;
        using modified_tuple = SelectedColumnsTuple;
    };
    
    template<typename Table, typename SelectedColumnsTuple>
    struct table_or_selected_columns<::foam::meta::typelist<Table>, SelectedColumnsTuple>
    {
        using type = typename metaspace::meta_table<Table>::template row_type<SelectedColumnsTuple>::type;
        using modified_tuple = SelectedColumnsTuple; 
    };
    
    //row_type
    template<typename TableList, typename SelectedColumnsTuple>
    struct row_type;
    
    template<typename TableList>
    struct row_type<TableList, std::tuple<>> : all_columns_of_all_tables<TableList> {};
    
    template<typename TableList, typename First, typename ...Rest>
    struct row_type<TableList, std::tuple<First, Rest...>> : table_or_selected_columns<TableList, std::tuple<First, Rest...>> {};
    

}}} //tagsql # developement # detail
