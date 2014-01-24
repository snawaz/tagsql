

#pragma once


#include <tagsql/test/table.h++>
#include <tagsql/test/tags_impl.h++>
#include <tagsql/test/tiny_types.h++>


namespace tagsql { namespace test { namespace universal_tags
{
	namespace detail
	{
		//get meta-functions for age
		template<typename Table>
		struct get_age;

		template<>
		struct get_age<schema::author_t> { using type = author_tag::age_t; };

		//get meta-functions for author_id
		template<typename Table>
		struct get_author_id;

		template<>
		struct get_author_id<schema::author_t> { using type = author_tag::author_id_t; };

		template<>
		struct get_author_id<schema::book_t> { using type = book_tag::author_id_t; };

		//get meta-functions for book_id
		template<typename Table>
		struct get_book_id;

		template<>
		struct get_book_id<schema::book_t> { using type = book_tag::book_id_t; };

		template<>
		struct get_book_id<schema::review_t> { using type = review_tag::book_id_t; };

		//get meta-functions for comment
		template<typename Table>
		struct get_comment;

		template<>
		struct get_comment<schema::review_t> { using type = review_tag::comment_t; };

		//get meta-functions for created
		template<typename Table>
		struct get_created;

		template<>
		struct get_created<schema::author_t> { using type = author_tag::created_t; };

		template<>
		struct get_created<schema::book_t> { using type = book_tag::created_t; };

		//get meta-functions for modified
		template<typename Table>
		struct get_modified;

		template<>
		struct get_modified<schema::author_t> { using type = author_tag::modified_t; };

		template<>
		struct get_modified<schema::book_t> { using type = book_tag::modified_t; };

		//get meta-functions for name
		template<typename Table>
		struct get_name;

		template<>
		struct get_name<schema::author_t> { using type = author_tag::name_t; };

		//get meta-functions for review_id
		template<typename Table>
		struct get_review_id;

		template<>
		struct get_review_id<schema::review_t> { using type = review_tag::review_id_t; };

		//get meta-functions for reviewer_id
		template<typename Table>
		struct get_reviewer_id;

		template<>
		struct get_reviewer_id<schema::review_t> { using type = review_tag::reviewer_id_t; };

		//get meta-functions for title
		template<typename Table>
		struct get_title;

		template<>
		struct get_title<schema::book_t> { using type = book_tag::title_t; };


	} //detail

	static const struct age_t
	{
		using _is_column = std::true_type;
		using _is_table  = std::false_type;
		using _is_unique = std::true_type;

		using tables = ::foam::meta::typelist<schema::author_t>;

		template<typename Table>
		struct get_column : detail::get_age<Table>{};

	}age{};

	static const struct author_t
	{
		using _is_column = std::false_type;
		using _is_table  = std::true_type;
		using _is_unique = null;

		using table = schema::author_t;

		author_tag::author_id_t                       author_id;               
		author_tag::name_t                            name;                    
		author_tag::age_t                             age;                     
		author_tag::modified_t                        modified;                
		author_tag::created_t                         created;                 
	}author{};

	static const struct author_id_t
	{
		using _is_column = std::true_type;
		using _is_table  = std::false_type;
		using _is_unique = std::false_type;

		using tables = ::foam::meta::typelist<schema::author_t,schema::book_t>;

		template<typename Table>
		struct get_column : detail::get_author_id<Table>{};

	}author_id{};

	static const struct book_t
	{
		using _is_column = std::false_type;
		using _is_table  = std::true_type;
		using _is_unique = null;

		using table = schema::book_t;

		book_tag::book_id_t                           book_id;                 
		book_tag::title_t                             title;                   
		book_tag::author_id_t                         author_id;               
		book_tag::modified_t                          modified;                
		book_tag::created_t                           created;                 
	}book{};

	static const struct book_id_t
	{
		using _is_column = std::true_type;
		using _is_table  = std::false_type;
		using _is_unique = std::false_type;

		using tables = ::foam::meta::typelist<schema::book_t,schema::review_t>;

		template<typename Table>
		struct get_column : detail::get_book_id<Table>{};

	}book_id{};

	static const struct comment_t
	{
		using _is_column = std::true_type;
		using _is_table  = std::false_type;
		using _is_unique = std::true_type;

		using tables = ::foam::meta::typelist<schema::review_t>;

		template<typename Table>
		struct get_column : detail::get_comment<Table>{};

	}comment{};

	static const struct created_t
	{
		using _is_column = std::true_type;
		using _is_table  = std::false_type;
		using _is_unique = std::false_type;

		using tables = ::foam::meta::typelist<schema::author_t,schema::book_t>;

		template<typename Table>
		struct get_column : detail::get_created<Table>{};

	}created{};

	static const struct modified_t
	{
		using _is_column = std::true_type;
		using _is_table  = std::false_type;
		using _is_unique = std::false_type;

		using tables = ::foam::meta::typelist<schema::author_t,schema::book_t>;

		template<typename Table>
		struct get_column : detail::get_modified<Table>{};

	}modified{};

	static const struct name_t
	{
		using _is_column = std::true_type;
		using _is_table  = std::false_type;
		using _is_unique = std::true_type;

		using tables = ::foam::meta::typelist<schema::author_t>;

		template<typename Table>
		struct get_column : detail::get_name<Table>{};

	}name{};

	static const struct review_t
	{
		using _is_column = std::false_type;
		using _is_table  = std::true_type;
		using _is_unique = null;

		using table = schema::review_t;

		review_tag::review_id_t                       review_id;               
		review_tag::book_id_t                         book_id;                 
		review_tag::reviewer_id_t                     reviewer_id;             
		review_tag::comment_t                         comment;                 
	}review{};

	static const struct review_id_t
	{
		using _is_column = std::true_type;
		using _is_table  = std::false_type;
		using _is_unique = std::true_type;

		using tables = ::foam::meta::typelist<schema::review_t>;

		template<typename Table>
		struct get_column : detail::get_review_id<Table>{};

	}review_id{};

	static const struct reviewer_id_t
	{
		using _is_column = std::true_type;
		using _is_table  = std::false_type;
		using _is_unique = std::true_type;

		using tables = ::foam::meta::typelist<schema::review_t>;

		template<typename Table>
		struct get_column : detail::get_reviewer_id<Table>{};

	}reviewer_id{};

	static const struct title_t
	{
		using _is_column = std::true_type;
		using _is_table  = std::false_type;
		using _is_unique = std::true_type;

		using tables = ::foam::meta::typelist<schema::book_t>;

		template<typename Table>
		struct get_column : detail::get_title<Table>{};

	}title{};

}}} //tagsql # test # universal_tags
