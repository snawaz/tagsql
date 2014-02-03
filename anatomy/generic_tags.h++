

#pragma once

namespace tagsql
{
	//given a list of tables and tag, resolve to table which the tag (column tag) belongs to
	template<typename TableList, typename GenericTag>
	struct resolve_table
	{
		using tables = typename GenericTag::tables::template intersection<TableList>::type;
		
		static_assert(tables::size != 0, "universal tag doesn't resolve to any of the tables.");
		static_assert(!(tables::size > 1), "ambiguous column : universal tag resolves to more than one tables.");
			
		using type = typename tables::template at<0>::type;
	};

	//given a list of tables and tag, resolve to tag of kind context_independent_tag.
	template<typename TableList, typename Column>
	struct resolve_column
	{
		template<typename GenericTag>
		struct resolver
		{
			using table = typename resolve_table<TableList, Column>::type;
			using type = typename GenericTag::template get_column<table>::type;
		};

		template<typename T>
		struct identity { using type = T; };

		template<typename T>
		static auto resolve(typename T::_is_unique *) -> resolver<T>;

		template<typename T>
		static auto resolve(...) -> identity<T>;

		using type = typename decltype(resolve<Column>(0))::type; 
	};

}
