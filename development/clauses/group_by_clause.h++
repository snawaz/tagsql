

#pragma once

#include <tagsql/development/anatomy/column.h++>

namespace tagsql { namespace development
{
    template<typename SelectQuery>
    class deferred_range;

	template<typename SelectQuery>
	class group_by_clause
	{
		public:
			using is_null_t = typename std::conditional<is_null<typename SelectQuery::group_by>::value, std::true_type, std::false_type>::type;
			
			template<typename ... Columns>
            auto group_by(Columns ... ) -> deferred_range<typename SelectQuery::template add_group_by<::foam::meta::typelist<Columns...>>::type>
            {
				static const std::vector<std::string> names { qualify(typename get_tag<Columns>::type()) ... };
				static const std::string clause = " GROUP BY " + ::tagsql::join(",", names) + " "; 

				auto range = static_cast<deferred_range<SelectQuery>*>(this);
                range->_query_without_select += clause;
				return { range->_connection, range->_query_without_select };
			}
	};

}} 
