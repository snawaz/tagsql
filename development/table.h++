

#pragma once


#include <foam/strlib/format.h>
#include <string>
#include <tagsql/development/column.h++>
#include <tagsql/development/tags.h++>
#include <tagsql/development/named_tuple.h++>

#define attribute

namespace tagsql { namespace development { namespace schema
{

#if 0	
    //This definition of schema for table 'book'.
    namespace txzi1 = ::tagsql::development::book_tag;
    struct book_t
    {
        column <txzi1::created_t>                     created;                 //std::string	-	-	-
        column <txzi1::modified_t>                    modified;                //std::string	-	-	-
        column <txzi1::author_id_t>                   author_id;               //long int	-	foreign_key	-
        column <txzi1::title_t>                       title;                   //std::string	-	-	-
        column <txzi1::book_id_t>                     book_id;                 //long int	non-nullable	primary_key	server_default
    };

    //This definition of schema for table 'author'.
    namespace txzi2 = ::tagsql::development::author_tag;
    struct author_t
    {
        column <txzi2::created_t>                     created;                 //std::string	-	-	-
        column <txzi2::modified_t>                    modified;                //std::string	-	-	-
        column <txzi2::age_t>                         age;                     //short	-	-	-
        column <txzi2::name_t>                        name;                    //std::string	-	-	-
        column <txzi2::author_id_t>                   author_id;               //long int	non-nullable	primary_key	server_default
    };

    //This definition of schema for table 'author'.
    namespace txzi3 = ::tagsql::development::review_tag;
    struct review_t
    {
        column <txzi3::review_id_t>                    review_id;                //std::string	-	-	-
        column <txzi3::reviewer_id_t>                  reviewer_id;                     //short	-	-	-
        column <txzi3::book_id_t>                      book_id;                    //std::string	-	-	-
        column <txzi3::comment_t>                      comment;               //long int	non-nullable	primary_key	server_default
    };
#else
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
#endif

	/*
    std::ostream& operator<<(std::ostream& out, book_t const & item)
    {
        return ::foam::strlib::print(out, "{0},{1},{2},{3},{4}", item.created,item.modified,item.author_id,item.title,item.book_id);
    }

    std::ostream& operator<<(std::ostream& out, author_t const & item)
    {
        return ::foam::strlib::print(out, "{0},{1},{2},{3},{4}", item.created,item.modified,item.age,item.name,item.author_id);
    }

    std::ostream& operator<<(std::ostream& out, review_t const & item)
    {
        return ::foam::strlib::print(out, "{0},{1},{2},{3}", item.review_id, item.reviewer_id, item.book_id, item.comment);
    }
	*/

}}} //tagsql # development # schema
