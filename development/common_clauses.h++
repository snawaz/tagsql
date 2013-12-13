

#pragma once

namespace tagsql { namespace development
{
	template<typename Bucket>
	class where_clause 
	{
		public:
			using is_null_t = typename std::conditional<is_null<typename Bucket::where>::value, std::true_type, std::false_type>::type;
			
			template<typename Condition>
            auto where(Condition const & expr) -> where_expression<typename Bucket::template add_where<Condition>::type>
            {
				static_assert(is_condition_expression<Condition>::value, 
						"Constraint Violation : expression passed to where() is invalid. It must be a condition expression involving column(s).");
				static_assert(expression_traits<Condition>::tables::template exists<typename Bucket::from>::value, 
						"Constraint Violation : table mismatch for the condition passed to from_expression::where().");

				auto range = reinterpret_cast<deferred_range<Bucket>*>(this);
                range->_query_without_select += " WHERE " + expr.repr();
				return { range->_connection, range->_query_without_select };
			}
	};

	template<typename Bucket>
	class group_by_clause
	{
		public:
			using is_null_t = typename std::conditional<is_null<typename Bucket::group_by>::value, std::true_type, std::false_type>::type;
	};

	template<typename Bucket>
	class having_clause
	{
		public:
			using is_null_t = typename std::conditional<is_null<typename Bucket::having>::value, std::true_type, std::false_type>::type;
	};

	template<typename Bucket>
	class order_by_clause
	{
		public:
			using is_null_t = typename std::conditional<is_null<typename Bucket::order_by>::value, std::true_type, std::false_type>::type;
	};

	template<typename Bucket>
	class limit_clause
	{
		public:
			using is_null_t = typename std::conditional<is_null<typename Bucket::limit>::value, std::true_type, std::false_type>::type;
	};

	template<typename Bucket>
	class offset_clause
	{
		public:
			using is_null_t = typename std::conditional<is_null<typename Bucket::offset>::value, std::true_type, std::false_type>::type;
	};

	template<typename Bucket>
	class fetch_clause
	{
		public:
			using is_null_t = typename std::conditional<is_null<typename Bucket::fetch>::value, std::true_type, std::false_type>::type;
	};

	struct empty {};

	template<typename Clause>
	using may_pick = typename std::conditional<Clause::is_null_t::value, Clause, empty>::type;

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
