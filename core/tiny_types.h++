

#pragma once

#include <type_traits>

namespace tagsql 
{
	struct null {};

	template<typename T>
	struct is_null : std::is_same<T,null> {};

	template<typename>
	struct always_wrong : std::false_type {};

#if 0 //GCC bug since 4.7.0 till 4.8.1 (current) : the brace-list-init doesn't evaluate from left-to-right, as required by the language specification (C++11)
	  //http://gcc.gnu.org/bugzilla/show_bug.cgi?id=51253
	  //http://stackoverflow.com/questions/14060264/order-of-evaluation-of-elements-in-list-initialization

	struct unpack
	{
		template<typename ...T>
		unpack(T && ...) {}
	};

#else //Workaround.

	struct _swallow_t { template<typename T> _swallow_t(T && ) {} };
	using unpack = _swallow_t[];

#endif 	

} // tagsql 
