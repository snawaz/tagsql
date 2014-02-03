
#pragma once

#include <string>
#include <tagsql/column.h++>
#include <tagsql/core/expressions/expression_fwd.h++>

namespace tagsql { namespace operators
{
	template<typename Tag>
	struct like_t
	{
		using literal_type = typename Tag::column_type;

		template<typename T, typename U>
		static auto eval(T && t, U && u) -> decltype( t == u )
		{
			return t == u;
		}

		static auto repr(std::string left, std::string right) -> std::string
		{
			return left + " = " + right;
		}

		auto like(literal_type pattern) const -> binary_expression<column_expression<Tag>, literal_expression<literal_type>, like_t<Tag>>
		{
			return { static_cast<column<Tag> const&>(*this), pattern }; 
		}
	};

}} //tagsql # operators
