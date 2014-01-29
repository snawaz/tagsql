

#pragma once

namespace tagsql 
{
    template<typename SelectQuery>
    class deferred_range;

	template<typename SelectQuery>
	class fetch_clause
	{
		public:
			using is_null_t = typename std::conditional<is_null<typename SelectQuery::fetch>::value, std::true_type, std::false_type>::type;
			
			template<typename Integral>
            auto fetch(Integral count) -> deferred_range<typename SelectQuery::template add_fetch<Integral>::type>
			{
				static_assert(std::is_integral<Integral>::value, "Argument to fetch() must be integral type.");
				std::size_t value = static_cast<std::size_t>(count);
				auto range = static_cast<deferred_range<SelectQuery>*>(this);
                range->_query_without_select += " FETCH FIRST " + std::to_string(value) + " ROWS ONLY";
				return { range->_connection, range->_query_without_select };
			}
	};

}
