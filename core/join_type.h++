

#pragma once

namespace tagsql
{
	namespace join_type
	{
		struct cross {};
		struct inner { static constexpr char const * const token = "JOIN"; };
		struct left  { static constexpr char const * const token = "LEFT JOIN";  };
		struct right { static constexpr char const * const token = "RIGHT JOIN"; };
		struct full  { static constexpr char const * const token = "FULL JOIN"; };
	}

}
