

#pragma once

namespace tagsql 
{
	namespace sql
	{
		const struct all_t{} all{};

		const struct order_by_direction_t { char const *token; } asc {"ASC"}, desc {"DESC"};
	}
	
	template<typename T>
	using _is_order_by_direction = std::is_same<sql::order_by_direction_t, T>;

} //tagsql
