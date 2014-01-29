

#pragma once

#include <stdexcept>

namespace tagsql
{
	struct syntax_error : public std::runtime_error
	{
		using std::runtime_error::runtime_error;
	};
	
	struct unsupported_type : public std::runtime_error
	{
		using std::runtime_error::runtime_error;
	};
}
