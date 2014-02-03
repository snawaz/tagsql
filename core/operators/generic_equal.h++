

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
	};

}
