

#pragma once

namespace tagsql
{
	template<typename Column>
	auto _is_column_expression_impl(typename Column::_is_column *) -> std::true_type;
	
	template<typename Column>
	auto _is_column_expression_impl(...) -> std::false_type;

	template<typename Column>
	using _is_column_expression = decltype(_is_column_expression_impl<Column>(0));

	template<typename Column>
	struct get_tag
	{
		static_assert(_is_column_expression<Column>::value, "Invalid column expression.");
	
		template<typename T>
		static auto tag_type(typename T::tag_type *) -> typename T::tag_type;
		
		template<typename T>
		static auto tag_type(...) -> T;

		using type = decltype(tag_type<Column>(0));
	};

} //tagsql
