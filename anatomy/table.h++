

#pragma once

namespace tagsql
{
	template<typename T>
	auto _is_table_expression_impl(typename T::table *) -> std::true_type;
	
	template<typename T>
	auto _is_table_expression_impl(...) -> std::false_type;

	template<typename T>
	using _is_table_expression = decltype(_is_table_expression_impl<T>(0));

	template<typename Tag>
	struct table_tag
	{
		static_assert(_is_table_expression<Tag>::value, "Tag is not a table expression. It must be either specific table or universal [table] tag");
		using type = typename Tag::table;
	};

	template<typename Tag>
	using table_tag_t = typename table_tag<Tag>::type;
}
