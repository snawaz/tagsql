

#pragma once

namespace tagsql { namespace development
{
    template<typename SelectQuery>
    class deferred_range;

	template<typename SelectQuery>
	class offset_clause
	{
		public:
			using is_null_t = typename std::conditional<is_null<typename SelectQuery::offset>::value, std::true_type, std::false_type>::type;
			
			template<typename Integral>
            auto offset(Integral count) -> deferred_range<typename SelectQuery::template add_offset<Integral>::type>
			{
				static_assert(std::is_integral<Integral>::value, 
						"Argument to offset() must be integral type.");

				auto range = static_cast<deferred_range<SelectQuery>*>(this);
				std::size_t value = static_cast<std::size_t>(count);
                range->_query_without_select += " OFFSET " + std::to_string(value);
				return { range->_connection, range->_query_without_select };
			}
	};
}} 
