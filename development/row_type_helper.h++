





#pragma once


#include <type_traits>

#include <tagsql/development/meta_table.h++>
#include <tagsql/development/meta_column.h++>
#include <foam/meta/typelist.h++>


namespace tagsql { namespace development { namespace detail
{
    template<typename T>
    using bare_type_t = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
    
	template<typename T>
	struct to_named_tuple;
	
	template<typename ...Tags>
	struct to_named_tuple<std::tuple<Tags...>> { using type = named_tuple<Tags...>; } ;

    //all_columns_of_all_tables
    template<typename TableList>
    struct all_columns_of_all_tables;
    
    template<typename ... Tables>
    struct all_columns_of_all_tables<::foam::meta::typelist<Tables...>> 
    {
		using stdtuple = bare_type_t<decltype(std::tuple_cat(typename metaspace::meta_table<Tables>::columns_tuple() ... ))>;
		using type = typename to_named_tuple<stdtuple>::type;
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
    

}}} //tagsql # development # detail
