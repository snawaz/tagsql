

#pragma once

#include <tagsql/development/clauses/common_clauses.h++>

namespace tagsql { namespace development
{
	template<typename Clause> struct _empty_t {}; //make it template so as to avoid having duplicate base classes. All the bases must be unique!

	template<typename Clause>
	using may_pick = typename std::conditional<Clause::is_null_t::value, Clause, _empty_t<Clause>>::type;

	template<typename SelectQuery>
	class clause_picker : public may_pick<where_clause<SelectQuery>>,
						  public may_pick<group_by_clause<SelectQuery>>,
						  public may_pick<having_clause<SelectQuery>>,
						  public may_pick<order_by_clause<SelectQuery>>,
						  public may_pick<limit_clause<SelectQuery>>,
						  public may_pick<offset_clause<SelectQuery>>,
						  public may_pick<fetch_clause<SelectQuery>>
	{

	};
}} 
