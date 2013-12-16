

#pragma once


namespace tagsql { namespace development
{
    template<typename Bucket>
    class deferred_range;
   
	template<typename Column>
	auto _is_valid_select_arg_impl(typename Column::_is_tag *) -> std::true_type;
	
	template<typename Column>
	auto _is_valid_select_arg_impl(...) -> std::false_type;

	template<typename Column>
	using _is_valid_select_arg = decltype(_is_valid_select_arg_impl<Column>(0));

	template<typename Column>
	struct get_tag
	{
		static_assert(_is_valid_select_arg<Column>::value, "Invalid tag in select()");
	
		template<typename T>
		static auto tag_type(typename T::tag_type *) -> typename T::tag_type;
		
		template<typename T>
		static auto tag_type(...) -> T;

		using type = decltype(tag_type<Column>(0));
	};

	namespace sql
	{
		const struct all_t{} all{};
		const struct order_by_direction_t { char const *token; } asc {"ASC"}, desc {"DESC"};
	}

	template<typename Bucket>
	class where_clause 
	{
		public:
			using is_null_t = typename std::conditional<is_null<typename Bucket::where>::value, std::true_type, std::false_type>::type;
			
			template<typename Condition>
            auto where(Condition const & expr) -> deferred_range<typename Bucket::template add_where<Condition>::type>
            {
				static_assert(is_condition_expression<Condition>::value, 
						"Constraint Violation : expression passed to where() is invalid. It must be a condition expression involving column(s).");
				static_assert(expression_traits<Condition>::tables::template exists<typename Bucket::from>::value, 
						"Constraint Violation : table mismatch for the condition passed to from_expression::where().");

				auto range = static_cast<deferred_range<Bucket>*>(this);
                range->_query_without_select += " WHERE " + expr.repr();
				return { range->_connection, range->_query_without_select };
			}
	};

	template<typename Bucket>
	class group_by_clause
	{
		public:
			using is_null_t = typename std::conditional<is_null<typename Bucket::group_by>::value, std::true_type, std::false_type>::type;
			
			template<typename ... Columns>
            auto group_by(Columns ... ) -> deferred_range<typename Bucket::template add_group_by<::foam::meta::typelist<Columns...>>::type>
            {
				static const std::vector<std::string> names { qualify(typename get_tag<Columns>::type()) ... };
				static const std::string clause = " GROUP BY " + ::tagsql::join(",", names) + " "; 

				auto range = static_cast<deferred_range<Bucket>*>(this);
                range->_query_without_select += clause;
				return { range->_connection, range->_query_without_select };
			}
	};

	template<typename Bucket>
	class having_clause
	{
		public:
			using is_null_t = typename std::conditional<is_null<typename Bucket::having>::value, std::true_type, std::false_type>::type;
			
			template<typename Condition>
            auto having(Condition const & expr) -> deferred_range<typename Bucket::template add_having<Condition>::type>
            {
				static_assert(is_condition_expression<Condition>::value, 
						"Constraint Violation : expression passed to where() is invalid. It must be a condition expression involving column(s).");
				static_assert(expression_traits<Condition>::tables::template exists<typename Bucket::from>::value, 
						"Constraint Violation : table mismatch for the condition passed to from_expression::where().");

				auto range = static_cast<deferred_range<Bucket>*>(this);
                range->_query_without_select += " HAVING " + expr.repr();
				return { range->_connection, range->_query_without_select };
			}
	};

	template<typename Bucket>
	class order_by_clause
	{
		public:
			using is_null_t = typename std::conditional<is_null<typename Bucket::order_by>::value, std::true_type, std::false_type>::type;
			
			template<typename Column>
            auto order_by(Column const & c) -> deferred_range<typename Bucket::template add_order_by<Column>::type>
			{
				auto range = static_cast<deferred_range<Bucket>*>(this);
                range->_query_without_select += " ORDER BY " + qualify(typename Column::tag_type());
				return { range->_connection, range->_query_without_select };
			}
			template<typename Column>
            auto order_by(Column const & c, sql::order_by_direction_t const &d) -> deferred_range<typename Bucket::template add_order_by<Column>::type>
			{
				auto range = static_cast<deferred_range<Bucket>*>(this);
                range->_query_without_select += " ORDER BY " + qualify(typename Column::tag_type()) + " " + d.token;
				return { range->_connection, range->_query_without_select };
			}
	};

	template<typename Bucket>
	class limit_clause
	{
		public:
			using is_null_t = typename std::conditional<is_null<typename Bucket::limit>::value, std::true_type, std::false_type>::type;
		
			template<typename Integral>
            auto limit(Integral count) -> deferred_range<typename Bucket::template add_limit<Integral>::type>
			{
				static_assert(std::is_integral<Integral>::value || std::is_same<sql::all_t, Integral>::value, 
						"Argument to limit() must be integral type OR sql::all.");

				auto range = static_cast<deferred_range<Bucket>*>(this);
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

	template<typename Bucket>
	class offset_clause
	{
		public:
			using is_null_t = typename std::conditional<is_null<typename Bucket::offset>::value, std::true_type, std::false_type>::type;
			
			template<typename Integral>
            auto offset(Integral count) -> deferred_range<typename Bucket::template add_offset<Integral>::type>
			{
				static_assert(std::is_integral<Integral>::value, 
						"Argument to offset() must be integral type.");

				auto range = static_cast<deferred_range<Bucket>*>(this);
				std::size_t value = static_cast<std::size_t>(count);
                range->_query_without_select += " OFFSET " + std::to_string(value);
				return { range->_connection, range->_query_without_select };
			}
	};

	template<typename Bucket>
	class fetch_clause
	{
		public:
			using is_null_t = typename std::conditional<is_null<typename Bucket::fetch>::value, std::true_type, std::false_type>::type;
			
			template<typename Integral>
            auto fetch(Integral count) -> deferred_range<typename Bucket::template add_fetch<Integral>::type>
			{
				static_assert(std::is_integral<Integral>::value, "Argument to fetch() must be integral type.");
				std::size_t value = static_cast<std::size_t>(count);
				auto range = static_cast<deferred_range<Bucket>*>(this);
                range->_query_without_select += " FETCH FIRST " + std::to_string(value) + " ROWS ONLY";
				return { range->_connection, range->_query_without_select };
			}
	};

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
