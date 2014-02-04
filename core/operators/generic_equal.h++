

#pragma once

#include <tagsql/core/operators/equal.h++>

namespace tagsql
{
	struct generic_equal_t
	{
		template<typename ContextIndependentTag>
		using operator_t = operators::equal_t<ContextIndependentTag>;

		static auto repr(std::string right, std::string left) -> std::string
		{
			return std::move(right) + " = " + std::move(left);
		}

		template<typename Left, typename Right>
		struct check_compatibility
		{
			template<typename L, typename R>
			static auto compare(L && l, R && r) -> decltype(l == r, std::true_type()); 

			static auto compare(...) -> std::false_type;

			static constexpr bool value = decltype(compare(std::declval<Left>(), std::declval<Right>()))::value;

			static_assert(!value, "incompatible operands : cannot be compared using == ");
		};
	};

}
