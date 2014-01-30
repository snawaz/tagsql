





#pragma once


#include <type_traits>

#include <tagsql/core/meta_table_base.h++>
#include <tagsql/core/meta_column.h++>
#include <tagsql/core/tiny_types.h++>
#include <foam/meta/typelist.h++>


namespace tagsql { namespace detail
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
	};
    
	template<typename Table>
	struct all_columns_of_all_tables<::foam::meta::typelist<Table>> 
	{
		using type = Table; 
	};
    
	//table or selected columns
	template<typename TableList, typename SelectedColumnsTuple>
	struct table_or_selected_columns;
    
	template<typename Table, typename ... Rest, typename SelectedColumnsTuple>
	struct table_or_selected_columns<::foam::meta::typelist<Table, Rest...>, SelectedColumnsTuple>
	{
		using possible_type = typename metaspace::meta_table<Table>::template row_type<SelectedColumnsTuple>::type;
		using type = typename std::conditional<std::is_same<possible_type, Table>::value, 
			  									Table, typename table_or_selected_columns<::foam::meta::typelist<Rest...>, SelectedColumnsTuple>::type>::type;
	};
    
	template<typename Table, typename SelectedColumnsTuple>
	struct table_or_selected_columns<::foam::meta::typelist<Table>, SelectedColumnsTuple>
	{
		using type = typename metaspace::meta_table<Table>::template row_type<SelectedColumnsTuple>::type;
	};
    
	//row_type
	template<typename TableList, typename SelectedColumnsTuple>
	struct row_type;
    
	template<typename TableList>
	struct row_type<TableList, std::tuple<>> : all_columns_of_all_tables<TableList> {};
    
#if 0	
	template<typename TableList, typename First, typename ...Rest>
	struct row_type<TableList, std::tuple<First, Rest...>> : table_or_selected_columns<TableList, std::tuple<First, Rest...>> {};
#else

	template<typename TableList, typename Column>
	struct resolve_column
	{
		template<typename UniversalTag>
		struct resolver
		{
			using tables = typename UniversalTag::tables::template intersection<TableList>::type;
	
			static_assert(tables::size != 0, "universal tag doesn't resolve to any of the tables.");
			static_assert(!(tables::size > 1), "ambiguous column : universal tag resolves to more than one tables.");
			
			using table = typename tables::template at<0>::type;
			using type = typename UniversalTag::template get_column<table>::type;
		};

		template<typename T>
		struct identity { using type = T; };

		template<typename T>
		static auto resolve(typename T::_is_unique *) -> resolver<T>;

		template<typename T>
		static auto resolve(...) -> identity<T>;

		using type = typename decltype(resolve<Column>(0))::type; 
	};
	
	template<typename TableList, typename First, typename ...Rest>
    struct row_type<TableList, std::tuple<First, Rest...>>
	{
		using type = typename table_or_selected_columns<TableList, 
											std::tuple<typename resolve_column<TableList, First>::type, typename resolve_column<TableList, Rest>::type...>>::type;
	};

#endif

}} //tagsql # detail
