

#pragma once

namespace tagsql { namespace operators
{
	struct and_t
	{
		template<typename T, typename U>
		static auto eval(T && t, U && u) -> decltype ( t && u )
		{
			return t && u;
		} 
	};

}}
