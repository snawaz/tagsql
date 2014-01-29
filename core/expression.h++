
#pragma once

#include <string>
#include <tagsql/column.h++>
#include <foam/meta/typelist.h++>

namespace tagsql
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


} // tagsql
