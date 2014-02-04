
#pragma once

#include <tagsql/core/tiny_types.h++>

namespace tagsql
{
	template<typename Tag>
	class column;
	
	struct productive_tag_t {};

	template<typename Tag>
	struct column_productive_tag_t : public productive_tag_t
	{
		using column_type = typename Tag::column_type;

		auto eval() const -> column_type
		{
			return static_cast<column<Tag> const&>(*this).value();
		}
		operator column_type() const 
		{ 
			return eval(); 
		}
		
		std::string repr() const
		{
			return Tag().repr();
		}
	};
}
