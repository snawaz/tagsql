
#include <string>
#include <tagsql/development/column.h++>
#include <tagsql/development/expression.h++>

namespace tagsql { namespace development
{
	namespace op
	{
		template<typename Tag>
		struct eq_t
		{
			template<typename T, typename U>
			static auto apply(T && t, U && u) -> decltype( t == u )
			{
				return t == u;
			}

			static constexpr char const * name = "=";

			using literal_type = typename Tag::column_type;

			auto operator==(literal_type value) const -> binary_expression<column_expression<Tag>, literal_expression<literal_type>, eq_t<Tag>>
			{
				return { static_cast<column<Tag> const&>(*this), value }; 
			}

			template<typename OtherTag>
			auto operator==(column<OtherTag> const & other) const -> binary_expression<column_expression<Tag>, column_expression<OtherTag>, eq_t<Tag>>
			{
				//todo: static checks!
				return { static_cast<column<Tag> const&>(*this), other }; 
			}
		};

		template<typename Tag>
		struct like_t
		{
			template<typename T, typename U>
			static auto apply(T && t, U && u) -> decltype( t == u )
			{
				return t == u;
			}

			static constexpr char const * name = "LIKE";

			using literal_type = typename Tag::column_type;

			auto like(literal_type pattern) const -> binary_expression<column_expression<Tag>, literal_expression<literal_type>, like_t<Tag>>
			{
				return { static_cast<column<Tag> const&>(*this), pattern }; 
			}
		};
	}

}}  //tagsql # development
