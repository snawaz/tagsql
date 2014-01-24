

#pragma once

#include <string>

namespace tagsql { namespace meta
{
    struct meta_column
	{
		std::string                      name;
		std::string                      type;
		std::string                      table;
		std::size_t                      character_max_length;
		std::size_t                      ordinal_position;
		bool                             is_nullable;
		bool                             has_server_default;
		bool                             is_primary_key;
		bool                             is_foreign_key;
		std::string                      ref_table_name;
		std::string                      ref_column_name;
	};

}}
