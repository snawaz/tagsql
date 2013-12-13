
#include <string>
#include <tagsql/development/column.h++>
#include <foam/meta/typelist.h++>

namespace tagsql { namespace development
{
	template<typename Tag>
	struct column_expression
	{
		column<Tag> const & _column;

		auto eval() const -> typename Tag::column_type
		{
			return _column.value();
		}

		std::string repr() const
		{
			return qualify(Tag());
		}
	};

	template<typename ValueType>
	struct literal_expression
	{
		ValueType _value;

		ValueType eval() const
		{
			return _value;
		}

		std::string repr() const
		{
			return _repr( std::is_arithmetic<ValueType>() );
		}

		std::string _repr(std::false_type) const { std::ostringstream ss; ss << _value; return "'" + ss.str() + "'"; }

		std::string _repr(std::true_type) const { return std::to_string(_value); }
	};

	struct and_t
	{
		template<typename T, typename U>
		static auto apply(T && t, U && u) -> decltype ( t && u )
		{
			return t && u;
		}

		static constexpr char const * name = "AND";
	};

	template<typename Left, typename Right, typename Operator>
	struct binary_expression
	{
		Left _left;
		Right _right;

		using eval_type = decltype(Operator::apply(std::declval<Left>().eval(), std::declval<Right>().eval()));

		auto eval() const -> eval_type
		{
			static bool value = Operator::apply(_left.eval(), _right.eval()); //ensure one time computation!
			return value;
		}

		std::string repr() const
		{
			static std::string value = _left.repr() + " " +  Operator::operator_symbol + " " + _right.repr();
			return value;
		}

		operator bool() const { return eval(); }

		operator std::string() const { return repr(); }

		template<typename OtherExpr>
		auto operator && (OtherExpr const & other) -> binary_expression<binary_expression<Left, Right, Operator>, OtherExpr, and_t>
		{
			return { *this, other };
		}
	};

	template<typename Expression>
	struct is_condition_expression : std::false_type {};
	
	template<typename Left, typename Right, typename Op>
	struct is_condition_expression<binary_expression<Left, Right, Op>> : std::true_type {};

	template<typename Expression>
	struct expression_traits;
	
	template<typename Left, typename Right, typename Op>
	struct expression_traits<binary_expression<Left, Right, Op>>
	{
		using tables = typename expression_traits<Left>::tables::template merge<typename expression_traits<Right>::tables>::type;
	};
	
	template<typename Tag>
	struct expression_traits<column_expression<Tag>>
	{
		using tables = ::foam::meta::typelist<typename Tag::table_type>;
	};

	template<typename ValueType>
	struct expression_traits<literal_expression<ValueType>>
	{
		using tables = ::foam::meta::typelist<>;
	};


}} // tagsql # development
