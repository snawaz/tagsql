
#pragma once

#include <tagsql/schema/table.h++>
#include <tagsql/schema/tags_impl.h++>

namespace tagsql { namespace universal_tags
{
	namespace detail
	{
		template<typename Table>
		struct get_created;
		
		template<>
		struct get_created<schema::book_t> { using type = book_tag::created_t; };
		
		template<>
		struct get_created<schema::author_t> { using type = author_tag::created_t; };
	}
	
	static const struct created_t
	{
		using _is_column     = std::true_type;
		using _is_table      = std::false_type;
		using _is_unique     = std::false_type;

		using tables       = ::foam::meta::typelist<schema::book_t, schema::author_t>;

		template<typename Table>
		struct get_column : detail::get_created<Table>{};
	
	}created;
	
	static const struct book_t
	{
		using _is_column    = std::false_type;
		using _is_table     = std::true_type;
		using _is_unique    = std::true_type;

		using table = tagsql::schema::book_t;
	}book;

}} //tagsql # universal_tags
