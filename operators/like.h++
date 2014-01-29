
#include <string>
#include <tagsql/column.h++>
#include <tagsql/core/expression.h++>

namespace tagsql
{
	namespace operators
	{
		template<typename Tag>
		struct like_t
		{
			template<typename T, typename U>
			static auto apply(T && t, U && u) -> decltype( t == u )
			{
				return t == u;
			}

			static constexpr char const * operator_symbol = "LIKE";

			using literal_type = typename Tag::column_type;

			auto like(literal_type pattern) const -> binary_expression<column_expression<Tag>, literal_expression<literal_type>, like_t<Tag>>
			{
				return { static_cast<column<Tag> const&>(*this), pattern }; 
			}
		};
	}

} //tagsql
