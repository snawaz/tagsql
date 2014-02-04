

#pragma once

#include <tagsql/core/row_type_helper.h++>
#include <tagsql/core/expressions/expression.h++>
#include <tagsql/core/operators/generic_all.h++>
#include <tagsql/anatomy/generic_tags.h++>
#include <tagsql/core/tiny_types.h++>

namespace tagsql
{
	template<typename Left, typename Right, typename Op>
	struct generic_binary_expression
	{
		Left _left;
		Right _right;

		template<typename TableList>
		std::string repr(TableList const & tablelist) const
		{
			return Op::repr(_left.repr(tablelist), _right.repr(tablelist));
		}

		template<typename T>
		operator T() const 
		{ 
			static_assert(always_wrong<T>::value, "expression involving generic tag(s) cannot produce value."); 
			return std::declval<T>(); //smarty! the linker will not even get a chance to look for the definition!
		}
	};

	template<typename Value>
	struct generic_expression
	{
	public:
		generic_expression(Value v) : _value(std::move(v)) {}

		template<typename TableList>
		std::string repr(TableList const & tablelist) const
		{
			return repr_impl(tablelist, static_cast<Value*>(0));
		}
	private:
		template<typename TableList, typename T, typename U = typename T::tag_category>
		std::string repr_impl(TableList const & tablelist, T*) const
		{
			return _value.repr(tablelist);
		}
		template<typename TableList>
		std::string repr_impl(TableList const &, ...) const
		{
			return literal_expression<Value> { _value }.repr();
		}
	private:
		Value _value;
	};
}
