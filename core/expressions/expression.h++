
#pragma once

#include <string>
#include <tagsql/column.h++>
#include <tagsql/core/operators/all.h++>
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

	template<typename Left, typename Right, typename Operator>
	struct binary_expression
	{
		using self = binary_expression<Left, Right, Operator>;

		Left _left;
		Right _right;

		using eval_type = decltype(Operator::eval(std::declval<Left>().eval(), std::declval<Right>().eval()));

		auto eval() const -> eval_type
		{
			return Operator::eval(_left.eval(), _right.eval()); 
		}

		std::string repr() const
		{
			return Operator::repr(_left.repr(), _right.repr());
		}

		operator bool() const { return eval(); }

		operator std::string() const { return repr(); }

		template<typename OtherExpr>
		auto operator && (OtherExpr const & other) -> binary_expression<self, OtherExpr, operators::and_t>
		{
			return { *this, other };
		}
	};

} // tagsql
