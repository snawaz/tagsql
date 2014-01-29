

#pragma once


#include <tagsql/keywords.h++>
#include <tagsql/anatomy/column.h++>

namespace tagsql 
{
    template<typename SelectQuery>
    class deferred_range;

	template<typename T>
	struct _is_order_by_argument : std::integral_constant<bool,_is_column_expression<T>::value || _is_order_by_direction<T>::value > {};
	
	template<>
	struct _is_order_by_argument<order_by_argument_t> : std::true_type {};

	template<template<typename> class Predicate, typename T, typename ...Rest>
	struct is_all : std::integral_constant<bool, Predicate<T>::value and is_all<Predicate, Rest...>::value> {};
	
	template<template<typename> class Predicate, typename T>
	struct is_all<Predicate,T> : std::integral_constant<bool, Predicate<T>::value> {};

	template<typename SelectQuery>
	class order_by_clause
	{
		public:
			using is_null_t = typename std::conditional<is_null<typename SelectQuery::order_by>::value, std::true_type, std::false_type>::type;
		
			template<typename ... OrderByArguments>
            auto order_by(OrderByArguments const &... args) -> deferred_range<typename SelectQuery::template add_order_by<::foam::meta::typelist<OrderByArguments...>>::type>
			{
				static_assert(sizeof...(args) != 0 && is_all<_is_order_by_argument, OrderByArguments...>::value, 
						"Invalid arguments to order_by()");

				//true, true, true, false, false, true, true, false, 
				auto range = static_cast<deferred_range<SelectQuery>*>(this);
                range->_query_without_select += order_by_impl(args...);
				return { range->_connection, range->_query_without_select };
			}
		private:
			template<typename Column, typename ...Rest>
			auto order_by_impl(Column const&, Rest const & ...rest) -> std::string
			{
				return " ORDER BY " + qualify(typename get_tag<Column>::type()) + iterate(std::false_type(), rest...);
			}
			template<typename ...Rest>
			auto order_by_impl(order_by_argument_t const & t, Rest const & ...rest) -> decltype(t.value)
			{
				return " ORDER BY " + t.value + iterate(std::false_type(), rest...);
			}
			template<typename Boolean>
			auto iterate(Boolean) -> std::string
			{
				return "";
			}
			template<typename Boolean, typename T, typename ...Rest>
			auto iterate(Boolean, T const & t, Rest const &...rest) -> std::string
			{
				using dispatch_token = std::integral_constant<bool,_is_order_by_direction<T>::value>;
				return (dispatch_token::value ? " " : ",") + static_dispatch(Boolean(), dispatch_token(), t, rest...);
			}
			template<typename AnyBoolean, typename T, typename ...Rest>
			auto static_dispatch(AnyBoolean, std::false_type, T const& t, Rest const &...rest) -> std::string
			{	
				return value(t,0) + iterate(std::false_type(), rest...);
			}
			template<typename T, typename ...Rest>
			auto static_dispatch(std::false_type, std::true_type, T const & t, Rest const &...rest) -> std::string
			{	
				return t.token + iterate(std::true_type(), rest...);
			}
			template<typename T, typename ...Rest>
			auto static_dispatch(std::true_type, std::true_type, T const&, Rest const & ...)
			{
				static_assert(always_wrong<T>::value, "Invalid arguments to order_by() : more than one consecutive order_by_direction is not allowed");
			}
			template<typename T>
			auto value(T const &t, int) -> decltype(t.value) 
			{	
				return t.value;
			}
			template<typename T>
			auto value(T const &, ...)-> std::string
			{	
				return qualify(typename get_tag<T>::type());
			}
	};
} 
