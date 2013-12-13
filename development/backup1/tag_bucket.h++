

#pragma once


namespace tagsql { namespace development
{
	template<
			typename Table, 
			typename OnCondition, 
			typename JoinType
			>
	struct join_tag
	{
		using table = Table;
		using condition = OnCondition;
		using type = JoinType;
	};

	template<
			typename SelectList,
			typename From,
			typename Where,
			typename JoinList,
			typename Limit,
			typename OrderBy,
			typename Offset,
			typename GroupBy
			>
	struct bucket_tag
	{
	};

}} //tagsql # development
