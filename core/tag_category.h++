

#pragma once

namespace tagsql { namespace core
{
	struct context_dependent_tag {};
	struct context_independent_tag {};
	struct context_independent_value_tag : context_independent_tag {};

}} // tagsql # core
