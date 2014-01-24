

#pragma once

#include <tagsql/development/expression.h++>

namespace tagsql { namespace development
{
    template<typename Bucket>
    class deferred_range;

	template<typename>
	struct always_wrong : std::false_type {};
   
	template<typename Column>
	auto _is_column_expression_impl(typename Column::_is_column *) -> std::true_type;
	
	template<typename Column>
	auto _is_column_expression_impl(...) -> std::false_type;

	template<typename Column>
	using _is_column_expression = decltype(_is_column_expression_impl<Column>(0));

	template<typename Column>
	struct get_tag
	{
		static_assert(_is_column_expression<Column>::value, "Invalid column expression.");
	
		template<typename T>
		static auto tag_type(typename T::tag_type *) -> typename T::tag_type;
		
		template<typename T>
		static auto tag_type(...) -> T;

		using type = decltype(tag_type<Column>(0));
	};

	namespace sql
	{
		const struct all_t{} all{};

		const struct order_by_direction_t { char const *token; } asc {"ASC"}, desc {"DESC"};
	}
	
	template<typename T>
	using _is_order_by_direction = std::is_same<sql::order_by_direction_t, T>;


	template<typename Bucket>
	class where_clause 
	{
		public:
			using is_null_t = typename std::conditional<is_null<typename Bucket::where>::value, std::true_type, std::false_type>::type;
			
			template<typename Condition>
            auto where(Condition const & expr) -> deferred_range<typename Bucket::template add_where<Condition>::type>
            {
				static_assert(is_condition_expression<Condition>::value, 
						"Constraint Violation : expression passed to where() is invalid. It must be a condition expression involving column(s).");
				static_assert(expression_traits<Condition>::tables::template exists<typename Bucket::from>::value, 
						"Constraint Violation : table mismatch for the condition passed to from_expression::where().");

				auto range = static_cast<deferred_range<Bucket>*>(this);
                range->_query_without_select += " WHERE " + expr.repr();
				return { range->_connection, range->_query_without_select };
			}
	};

	template<typename Bucket>
	class group_by_clause
	{
		public:
			using is_null_t = typename std::conditional<is_null<typename Bucket::group_by>::value, std::true_type, std::false_type>::type;
			
			template<typename ... Columns>
            auto group_by(Columns ... ) -> deferred_range<typename Bucket::template add_group_by<::foam::meta::typelist<Columns...>>::type>
            {
				static const std::vector<std::string> names { qualify(typename get_tag<Columns>::type()) ... };
				static const std::string clause = " GROUP BY " + ::tagsql::join(",", names) + " "; 

				auto range = static_cast<deferred_range<Bucket>*>(this);
                range->_query_without_select += clause;
				return { range->_connection, range->_query_without_select };
			}
	};

	template<typename Bucket>
	class having_clause
	{
		public:
			using is_null_t = typename std::conditional<is_null<typename Bucket::having>::value, std::true_type, std::false_type>::type;
			
			template<typename Condition>
            auto having(Condition const & expr) -> deferred_range<typename Bucket::template add_having<Condition>::type>
            {
				static_assert(is_condition_expression<Condition>::value, 
						"Constraint Violation : expression passed to where() is invalid. It must be a condition expression involving column(s).");
				static_assert(expression_traits<Condition>::tables::template exists<typename Bucket::from>::value, 
						"Constraint Violation : table mismatch for the condition passed to from_expression::where().");

				auto range = static_cast<deferred_range<Bucket>*>(this);
                range->_query_without_select += " HAVING " + expr.repr();
				return { range->_connection, range->_query_without_select };
			}
	};
	
	template<typename T>
	struct _is_order_by_argument : std::integral_constant<bool,_is_column_expression<T>::value || _is_order_by_direction<T>::value > {};
	
	template<>
	struct _is_order_by_argument<order_by_argument_t> : std::true_type {};

	template<template<typename> class Predicate, typename T, typename ...Rest>
	struct is_all : std::integral_constant<bool, Predicate<T>::value and is_all<Predicate, Rest...>::value> {};
	
	template<template<typename> class Predicate, typename T>
	struct is_all<Predicate,T> : std::integral_constant<bool, Predicate<T>::value> {};

	template<typename Bucket>
	class order_by_clause
	{
		public:
			using is_null_t = typename std::conditional<is_null<typename Bucket::order_by>::value, std::true_type, std::false_type>::type;
		
			template<typename ... OrderByArguments>
            auto order_by(OrderByArguments const &... args) -> deferred_range<typename Bucket::template add_order_by<::foam::meta::typelist<OrderByArguments...>>::type>
			{
				static_assert(sizeof...(args) != 0 && is_all<_is_order_by_argument, OrderByArguments...>::value, 
						"Invalid arguments to order_by()");

				//true, true, true, false, false, true, true, false, 
				auto range = static_cast<deferred_range<Bucket>*>(this);
                range->_query_without_select += order_by_impl(args...);
				return { range->_connection, range->_query_without_select };
			}
		private:
#if 1			
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
#else
			template<typename Column, typename ...Rest>
			auto order_by_impl(Column const&, Rest const & ...rest) -> std::string
			{
				return " ORDER BY " + qualify(typename get_tag<Column>::type()) + iterate(std::true_type(), rest...);
			}
			
			template<typename Boolean>
			auto iterate(Boolean) -> std::string
			{
				return "";
			}
			template<typename Boolean, typename T, typename ...Rest>
			auto iterate(Boolean, T const & t, Rest const &...rest) -> std::string
			{
				using dispatch_token = std::integral_constant<bool,_is_column_expression<T>::value>;
				return (dispatch_token::value ? ", " : " ") + static_dispatch(Boolean(), dispatch_token(), t, rest...);
			}

			template<typename AnyBoolean, typename T, typename ...Rest>
			auto static_dispatch(AnyBoolean, std::true_type, T const&, Rest const &...rest) -> std::string
			{	
				return qualify(typename get_tag<T>::type()) + iterate(std::true_type(), rest...);
			}
			template<typename T, typename ...Rest>
			auto static_dispatch(std::true_type, std::false_type, T const & t, Rest const &...rest) -> std::string
			{	
				return t.token + iterate(std::false_type(), rest...);
			}
			template<typename T, typename ...Rest>
			auto static_dispatch(std::false_type, std::false_type, T const&, Rest const & ...)
			{
				static_assert(always_wrong<T>::value, "Invalid arguments to order_by() : more than one consecutive order_by_direction is not allowed");
			}
#endif
	};

	template<typename Bucket>
	class limit_clause
	{
		public:
			using is_null_t = typename std::conditional<is_null<typename Bucket::limit>::value, std::true_type, std::false_type>::type;
		
			template<typename Integral>
            auto limit(Integral count) -> deferred_range<typename Bucket::template add_limit<Integral>::type>
			{
				static_assert(std::is_integral<Integral>::value || std::is_same<sql::all_t, Integral>::value, 
						"Argument to limit() must be integral type OR sql::all.");

				auto range = static_cast<deferred_range<Bucket>*>(this);
                range->_query_without_select += " LIMIT " + to_string(count, std::is_integral<Integral>() );
				return { range->_connection, range->_query_without_select };
			}
		private:
			template<typename Integral>
            auto to_string(Integral count, std::true_type) -> std::string
			{
				std::size_t value = static_cast<std::size_t>(count);
                return std::to_string(value);
			}
            auto to_string(sql::all_t const &, std::false_type) -> std::string
			{
				return "ALL";
			}
	};

	template<typename Bucket>
	class offset_clause
	{
		public:
			using is_null_t = typename std::conditional<is_null<typename Bucket::offset>::value, std::true_type, std::false_type>::type;
			
			template<typename Integral>
            auto offset(Integral count) -> deferred_range<typename Bucket::template add_offset<Integral>::type>
			{
				static_assert(std::is_integral<Integral>::value, 
						"Argument to offset() must be integral type.");

				auto range = static_cast<deferred_range<Bucket>*>(this);
				std::size_t value = static_cast<std::size_t>(count);
                range->_query_without_select += " OFFSET " + std::to_string(value);
				return { range->_connection, range->_query_without_select };
			}
	};

	template<typename Bucket>
	class fetch_clause
	{
		public:
			using is_null_t = typename std::conditional<is_null<typename Bucket::fetch>::value, std::true_type, std::false_type>::type;
			
			template<typename Integral>
            auto fetch(Integral count) -> deferred_range<typename Bucket::template add_fetch<Integral>::type>
			{
				static_assert(std::is_integral<Integral>::value, "Argument to fetch() must be integral type.");
				std::size_t value = static_cast<std::size_t>(count);
				auto range = static_cast<deferred_range<Bucket>*>(this);
                range->_query_without_select += " FETCH FIRST " + std::to_string(value) + " ROWS ONLY";
				return { range->_connection, range->_query_without_select };
			}
	};

}} 
