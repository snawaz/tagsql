
#pragma once

#include <string>
#include <tagsql/column.h++>
#include <tagsql/core/expressions/expression_fwd.h++>

namespace tagsql { namespace operators
{
	template<typename Tag>
	struct equal_t
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
#if 1
		auto operator==(literal_type value) const -> binary_expression<column_expression<Tag>, literal_expression<literal_type>, equal_t<Tag>>
		{
			return { static_cast<column<Tag> const&>(*this), value }; 
		}

		template<typename OtherTag>
		auto operator==(column<OtherTag> const & other) const -> binary_expression<column_expression<Tag>, column_expression<OtherTag>, equal_t<Tag>>
		{
			//todo: static checks!
			return { static_cast<column<Tag> const&>(*this), other }; 
		}
#else
		auto operator==(literal_type value) const -> binary_expression<column_expression<Tag>, literal_expression<literal_type>, equal_t<Tag>>
		{
			return { static_cast<column<Tag> const&>(*this), value }; 
		}

	private:
#endif		
	};

}} //tagsql # operators
