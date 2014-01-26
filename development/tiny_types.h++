

#pragma once

#include <type_traits>

namespace tagsql { namespace development
{
	struct null {};

	template<typename T>
	struct is_null : std::is_same<T,null> {};

	template<typename>
	struct always_wrong : std::false_type {};
   
	struct sink
	{
		template<typename ...T>
		sink(T && ...) {}
	};

}} // tagsql # development
