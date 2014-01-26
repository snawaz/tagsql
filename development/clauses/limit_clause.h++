

#pragma once

#include <tagsql/development/keywords.h++>

namespace tagsql { namespace development
{
    template<typename SelectQuery>
    class deferred_range;

	template<typename SelectQuery>
	class limit_clause
	{
		public:
			using is_null_t = typename std::conditional<is_null<typename SelectQuery::limit>::value, std::true_type, std::false_type>::type;
		
			template<typename Integral>
            auto limit(Integral count) -> deferred_range<typename SelectQuery::template add_limit<Integral>::type>
			{
				static_assert(std::is_integral<Integral>::value || std::is_same<sql::all_t, Integral>::value, 
						"Argument to limit() must be integral type OR sql::all.");

				auto range = static_cast<deferred_range<SelectQuery>*>(this);
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
}} 
