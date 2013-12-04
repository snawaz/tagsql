
#pragma once

#include <vector>
#include <string>

namespace tagsql { namespace meta 
{
    struct meta_table
    {
	std::string              name;
	std::vector<meta_column> columns;
    };

}}
