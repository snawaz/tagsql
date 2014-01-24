

#pragma once


#include <foam/strlib/format.h>
#include <string>
#include <tagsql/development/column.h++>
#include <tagsql/development/tags.h++>
#include <tagsql/development/named_tuple.h++>

namespace tagsql { namespace development { namespace schema
{

    //This definition of schema for table 'book'.
    namespace txzi1 = ::tagsql::development::book_tag;
	using _book_base_t = named_tuple<txzi1::book_id_t, txzi1::title_t, txzi1::author_id_t, txzi1::modified_t, txzi1::created_t>;
    struct book_t : public _book_base_t
    {
		using _book_base_t::_book_base_t;
    };

    //This definition of schema for table 'author'.
    namespace txzi2 = ::tagsql::development::author_tag;
	using _author_base_t = named_tuple<txzi2::author_id_t, txzi2::name_t, txzi2::age_t, txzi2::modified_t, txzi2::created_t>;
    struct author_t : public _author_base_t
    {
		using _author_base_t::_author_base_t;
    };

    //This definition of schema for table 'review'.
    namespace txzi3 = ::tagsql::development::review_tag;
	using _review_base_t = named_tuple<txzi3::review_id_t, txzi3::reviewer_id_t, txzi3::book_id_t, txzi3::comment_t>;
	struct review_t : public _review_base_t 
	{
		using _review_base_t::_review_base_t;
	};

}}} //tagsql # development # schema
