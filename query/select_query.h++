

#pragma once

#include <tagsql/tiny_types.h++>
#include <foam/meta/typelist.h++>

namespace tagsql 
{
	namespace join_type
	{
		struct cross {};
		struct inner { static constexpr char const * const token = "JOIN"; };
		struct left  { static constexpr char const * const token = "LEFT JOIN";  };
		struct right { static constexpr char const * const token = "RIGHT JOIN"; };
		struct full  { static constexpr char const * const token = "FULL JOIN"; };
	}

	template<
			typename Table, 
			typename JoinType,
			typename OnCondition = null 
			>
	struct join_tag
	{
		using table = Table;
		using condition = OnCondition;
		using join_type = JoinType;
	};

	struct dummy {};

	template<typename T>
	using maybe = typename std::conditional<is_null<T>::value, dummy, T>::type;

	template<
			typename Select,                                     //std::tuple of selected columms
			typename From      = null,                           //from table
			typename Joins     = ::foam::meta::typelist<>,       //typelist of join tags
			typename Ons       = ::foam::meta::typelist<>,       //typelist of ON conditions 
			typename Where     = null,                           //where condition
			typename GroupBy   = null,                           //group_by
			typename Having    = null,                           //having_by
			typename OrderBy   = null,                           //order_by
			typename Limit     = null,                           //limit
			typename Offset    = null,                           //offset
			typename Fetch     = null
			>
	struct select_query 
	{

		using select     = Select;
		using from       = From;
		using joins      = Joins;
		using ons        = Ons;
		using where      = Where;
		using group_by   = GroupBy;
		using having     = Having;
		using order_by   = OrderBy;
		using limit      = Limit;
		using offset     = Offset;
		using fetch      = Fetch;

		template<typename JoinTag>
		struct select_table { using type = typename JoinTag::table; };

		using join_tables     = typename joins::template transform<select_table>::type;
		using all_tables      = typename join_tables::template push_back<from>::type;

		template<typename Table>
		struct add_from
		{
			using type = select_query<Select,Table,Joins,Ons,Where,GroupBy,Having,OrderBy,Limit,Offset,Fetch>;
		};

		template<typename Table, typename JoinType>
		struct add_join
		{
			using TJoins = typename Joins::template push_back<join_tag<Table,JoinType>>::type;
			using type = select_query<Select,From,TJoins,Ons,Where,GroupBy,Having,OrderBy,Limit,Offset,Fetch>;
		};
		
		template<typename TOn>
		struct add_on
		{
			using TOns = typename Ons::template push_back<TOn>::type;
			using type = select_query<Select,From,Joins,TOns,Where,GroupBy,Having,OrderBy,Limit,Offset,Fetch>;
		};
		
		template<typename TWhere>
		struct add_where
		{
			using type = select_query<Select,From,Joins,Ons,TWhere,GroupBy,Having,OrderBy,Limit,Offset,Fetch>;
		};

		template<typename TGroupBy>
		struct add_group_by
		{
			static_assert(is_null<GroupBy>(), "Invalid query : group_by() is allowed exactly once.");
			using type = select_query<Select,From,Joins,Ons,Where,TGroupBy,Having,OrderBy,Limit,Offset,Fetch>;
		};
		
		template<typename THaving>
		struct add_having
		{
			static_assert(is_null<Having>(), "Invalid query : group_by() is allowed exactly once.");
			using type = select_query<Select,From,Joins,Ons,Where,GroupBy,THaving,OrderBy,Limit,Offset,Fetch>;
		};
		
		template<typename TOrderBy>
		struct add_order_by
		{
			static_assert(is_null<OrderBy>(), "Invalid query : order_by() is allowed exactly once.");
			using type = select_query<Select,From,Joins,Ons,Where,GroupBy,Having,TOrderBy,Limit,Offset,Fetch>;
		};

		template<typename TLimit>
		struct add_limit
		{
			static_assert(is_null<Limit>(), "Invalid query : limit() is allowed exactly once.");
			using type = select_query<Select,From,Joins,Ons,maybe<Where>,maybe<GroupBy>,maybe<Having>,maybe<OrderBy>,TLimit,Offset,maybe<Fetch>>;
		};

		template<typename TOffset>
		struct add_offset
		{
			static_assert(is_null<Offset>(), "Invalid query : offset() is allowed exactly once.");
			using type = select_query<Select,From,Joins,Ons,maybe<Where>,maybe<GroupBy>,maybe<Having>,maybe<OrderBy>,maybe<Limit>,TOffset,Fetch>;
		};

		template<typename TFetch>
		struct add_fetch
		{
			static_assert(is_null<Fetch>(), "Invalid query : offset() is allowed exactly once.");
			
			/*
			 * http://www.postgresql.org/docs/9.1/static/sql-select.html
			 *   "According to the standard, the OFFSET clause must come before the FETCH clause if both are present; 
			 *    but PostgreSQL is laxer and allows either order."
			 * So we follow what the Standard says here: enforce FETCH to come after OFFSET.
			 */

			using type = select_query<Select,From,Joins,Ons,maybe<Where>,maybe<GroupBy>,maybe<Having>,maybe<OrderBy>,maybe<Limit>,maybe<Offset>,TFetch>;
		};
	};

} //tagsql
