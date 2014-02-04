
#pragma once

#include <tagsql/core/tiny_types.h++>

namespace tagsql
{
	struct unproductive_tag_t
	{
		template<typename T>
		T eval() const
		{
			static_assert(always_wrong<T>::value, 
					"Wrong usage : content_dependent_tag/context_independent_tag cannot produce value. Only context_independent_value_tag can produce value"); 
			return std::declval<T>(); //smarty! the linker will not even get a chance to look for the definition!
		}
		template<typename T>
		operator T() const 
		{ 
			return eval<T>(); 
		}
	};
}
