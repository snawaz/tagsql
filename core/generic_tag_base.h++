
#pragma once

#include <tagsql/anatomy/generic_tags.h++>
#include <tagsql/core/unproductive_tag.h++>
#include <tagsql/core/expressions/expression.h++>
#include <tagsql/core/expressions/generic_binary_expression.h++>

namespace tagsql
{
	template<typename GenericTag>
	struct generic_tag_expression_t : public unproductive_tag_t
	{
		template<typename TableList>
		std::string repr(TableList) const 
		{
			using table = typename resolve_table<TableList, GenericTag>::type;
			return qualify(typename GenericTag::template get_column<table>::type()); 
		}
	};

	template<typename GenericTag>
	struct generic_tag_base_t : public generic_tag_expression_t<GenericTag>
	{
		template<typename Other> //Other could be tag or literal value
		auto operator==(Other const & value) const -> generic_binary_expression<GenericTag, generic_expression<Other>, generic_equal_t>
		{
			return {static_cast<GenericTag const&>(*this), value};
		}
	};
}
