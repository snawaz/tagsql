

#pragma once

namespace tagsql { namespace development
{
    template<typename SelectQuery>
    class deferred_range;
template<typename SelectQuery>
	class where_clause 
	{
		public:
			using is_null_t = typename std::conditional<is_null<typename SelectQuery::where>::value, std::true_type, std::false_type>::type;
			
			template<typename Condition>
            auto where(Condition const & expr) -> deferred_range<typename SelectQuery::template add_where<Condition>::type>
            {
				static_assert(is_condition_expression<Condition>::value, 
						"Constraint Violation : expression passed to where() is invalid. It must be a condition expression involving column(s).");
				static_assert(expression_traits<Condition>::tables::template exists<typename SelectQuery::from>::value, 
						"Constraint Violation : table mismatch for the condition passed to from_expression::where().");

				auto range = static_cast<deferred_range<SelectQuery>*>(this);
                range->_query_without_select += " WHERE " + expr.repr();
				return { range->_connection, range->_query_without_select };
			}
	};
}} 
