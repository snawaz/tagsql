
#include <string>
#include <tagsql/column.h++>
#include <tagsql/core/expression.h++>

namespace tagsql
{
	namespace operators
	{
		template<typename Tag>
		struct equal_t
		{
			template<typename T, typename U>
			static auto apply(T && t, U && u) -> decltype( t == u )
			{
				return t == u;
			}

			static constexpr char const * operator_symbol = "=";

			using literal_type = typename Tag::column_type;

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

			template<typename OtherTag> //, typename X = typename OtherTag::column_type>
			auto operator==(OtherTag const & other) const -> binary_expression<column_expression<Tag>, column_expression<OtherTag>, equal_t<Tag>>
			{
				//todo: static checks!
				return { static_cast<column<Tag> const&>(*this), column<OtherTag>{} }; 
			}
		};
	}

} //tagsql
