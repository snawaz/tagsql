
#pragma once

#include <tagsql/core/expression.h++>

namespace tagsql
{

	template<typename Expression>
	struct is_condition_expression : std::false_type {};
	
	template<typename Left, typename Right, typename Op>
	struct is_condition_expression<binary_expression<Left, Right, Op>> : std::true_type {};

	template<typename Expression>
	struct expression_traits;
	
	template<typename Left, typename Right, typename Op>
	struct expression_traits<binary_expression<Left, Right, Op>>
	{
		using tables = typename expression_traits<Left>::tables::template merge<typename expression_traits<Right>::tables>::type;
	};
	
	template<typename Tag>
	struct expression_traits<column_expression<Tag>>
	{
		using tables = ::foam::meta::typelist<typename Tag::table_type>;
	};

	template<typename ValueType>
	struct expression_traits<literal_expression<ValueType>>
	{
		using tables = ::foam::meta::typelist<>;
	};

} // tagsql
