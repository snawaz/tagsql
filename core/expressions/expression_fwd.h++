

#pragma once

namespace tagsql
{
	template<typename Left, typename Right, typename Operator>
	struct binary_expression;
	
	template<typename Tag>
	struct column_expression;
	
	template<typename ValueType>
	struct literal_expression;
}

