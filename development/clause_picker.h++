

#pragma once

#include <tagsql/development/common_clauses.h++>

namespace tagsql { namespace development
{
	template<typename Clause> struct empty {}; //make it template so as to avoid having duplicate base classes. All the bases must be unique!

	template<typename Clause>
	using may_pick = typename std::conditional<Clause::is_null_t::value, Clause, empty<Clause>>::type;

	template<typename Bucket>
	class clause_picker : public may_pick<where_clause<Bucket>>,
						  public may_pick<group_by_clause<Bucket>>,
						  public may_pick<having_clause<Bucket>>,
						  public may_pick<order_by_clause<Bucket>>,
						  public may_pick<limit_clause<Bucket>>,
						  public may_pick<offset_clause<Bucket>>,
						  public may_pick<fetch_clause<Bucket>>
	{

	};
}} 
