

#pragma once


#include <tagsql/core/tag_category.h++>
#include <tagsql/core/tiny_types.h++>
#include <tagsql/core/expressions/expression.h++>
#include <tagsql/core/expressions/generic_binary_expression.h++>
#include <tagsql/core/meta_table_base.h++>
#include <tagsql/anatomy/generic_tags.h++>
#include <testing/fest/tags_impl.h++>
#include <testing/fest/table.h++>


namespace snawaz { namespace db { namespace fest { namespace generic_tags
{
	namespace detail
	{
		//get meta-functions for age
		template<typename Table>
		struct get_age;

		template<>
		struct get_age<schema::author_t> { using type = author_tag::age_t; };

		//get meta-functions for author
		template<typename Table>
		struct get_author;

		template<>
		struct get_author<schema::author_t> { using type = author_tag::author_t; };

		template<>
		struct get_author<schema::book_t> { using type = book_tag::author_t; };

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

	template<typename GenericTag>
	struct _generic_tag_base_t
	{
		template<typename TableList>
		std::string repr(TableList) const 
		{
			using table = typename ::tagsql::resolve_table<TableList, GenericTag>::type;
			return qualify(typename GenericTag::template get_column<table>::type()); 
		}

		template<typename Other> //Other could be tag or literal value
		auto operator==(Other const & value) const -> ::tagsql::generic_binary_expression<GenericTag, ::tagsql::generic_expression<Other>, tagsql::generic_equal_t>
		{
			return {static_cast<GenericTag const&>(*this), value};
		}
	};

	static struct age_g : _generic_tag_base_t<age_g>
	{
		using _is_column = std::true_type;
		using _is_table  = std::false_type;
		using _is_unique = std::true_type;

		using tag_category  = ::tagsql::core::context_dependent_tag;
		using tables = ::foam::meta::typelist<schema::author_t>;

		template<typename Table>
		struct get_column : detail::get_age<Table>{};

#if 0		
		template<typename TableList>
		std::string repr(TableList) const 
		{
			using table = typename tagsql::resolve_table<TableList, age_g>::type;
			return qualify(typename get_column<table>::type()); 
		}

		template<typename Other> //Other could be tag or literal value
		auto operator==(Other const & value) const -> tagsql::generic_binary_expression<age_g, tagsql::literal_expression<Other>, tagsql::generic_equal_t>
		{
			return {*this, value};
		}
#endif

	}age{};

	static struct author_g : _generic_tag_base_t<author_g>
	{
		using _is_column = std::true_type;
		using _is_table  = std::true_type;
		using _is_unique = ::tagsql::null;

		using tag_category  = ::tagsql::core::context_dependent_tag;
		using tables = ::foam::meta::typelist<schema::author_t,schema::book_t>;

		template<typename Table>
		struct get_column : detail::get_author<Table>{};
#if 0
		template<typename TableList>
		std::string repr(TableList) const 
		{
			using table = typename ::tagsql::resolve_table<TableList, author_g>::type;
			return qualify(typename get_column<table>::type()); 
		}

		template<typename Other> //Other could be tag or literal value
		auto operator==(Other const & value) const -> ::tagsql::generic_binary_expression<author_g, ::tagsql::generic_expression<Other>, tagsql::generic_equal_t>
		{
			return {*this, value};
		}
#endif
		using table = schema::author_t;

		author_tag::author_id_t                       author_id;               
		author_tag::name_t                            name;                    
		author_tag::age_t                             age;                     
		author_tag::modified_t                        modified;                
		author_tag::created_t                         created;                 
		author_tag::author_t                          author;                  

	}author{};

	static struct author_id_g
	{
		using _is_column = std::true_type;
		using _is_table  = std::false_type;
		using _is_unique = std::false_type;

		using tag_category  = ::tagsql::core::context_dependent_tag;
		using tables = ::foam::meta::typelist<schema::author_t,schema::book_t>;

		template<typename Table>
		struct get_column : detail::get_author_id<Table>{};

	}author_id{};

	static struct book_g
	{
		using _is_column = std::false_type;
		using _is_table  = std::true_type;
		using _is_unique = ::tagsql::null;

		using table = schema::book_t;

		book_tag::book_id_t                           book_id;                 
		book_tag::title_t                             title;                   
		book_tag::author_id_t                         author_id;               
		book_tag::modified_t                          modified;                
		book_tag::created_t                           created;                 
		book_tag::author_t                            author;                  

	}book{};

	static struct book_id_g
	{
		using _is_column = std::true_type;
		using _is_table  = std::false_type;
		using _is_unique = std::false_type;

		using tag_category  = ::tagsql::core::context_dependent_tag;
		using tables = ::foam::meta::typelist<schema::book_t,schema::review_t>;

		template<typename Table>
		struct get_column : detail::get_book_id<Table>{};

	}book_id{};

	static struct comment_g
	{
		using _is_column = std::true_type;
		using _is_table  = std::false_type;
		using _is_unique = std::true_type;

		using tag_category  = ::tagsql::core::context_dependent_tag;
		using tables = ::foam::meta::typelist<schema::review_t>;

		template<typename Table>
		struct get_column : detail::get_comment<Table>{};

	}comment{};

	static struct created_g
	{
		using _is_column = std::true_type;
		using _is_table  = std::false_type;
		using _is_unique = std::false_type;

		using tag_category  = ::tagsql::core::context_dependent_tag;
		using tables = ::foam::meta::typelist<schema::author_t,schema::book_t>;

		template<typename Table>
		struct get_column : detail::get_created<Table>{};

	}created{};

	static struct modified_g
	{
		using _is_column = std::true_type;
		using _is_table  = std::false_type;
		using _is_unique = std::false_type;

		using tag_category  = ::tagsql::core::context_dependent_tag;
		using tables = ::foam::meta::typelist<schema::author_t,schema::book_t>;

		template<typename Table>
		struct get_column : detail::get_modified<Table>{};

	}modified{};

	static struct name_g
	{
		using _is_column = std::true_type;
		using _is_table  = std::false_type;
		using _is_unique = std::true_type;

		using tag_category  = ::tagsql::core::context_dependent_tag;
		using tables = ::foam::meta::typelist<schema::author_t>;

		template<typename Table>
		struct get_column : detail::get_name<Table>{};

	}name{};

	static struct review_g
	{
		using _is_column = std::false_type;
		using _is_table  = std::true_type;
		using _is_unique = ::tagsql::null;

		using table = schema::review_t;

		review_tag::review_id_t                       review_id;               
		review_tag::book_id_t                         book_id;                 
		review_tag::reviewer_id_t                     reviewer_id;             
		review_tag::comment_t                         comment;                 

	}review{};

	static struct review_id_g
	{
		using _is_column = std::true_type;
		using _is_table  = std::false_type;
		using _is_unique = std::true_type;

		using tag_category  = ::tagsql::core::context_dependent_tag;
		using tables = ::foam::meta::typelist<schema::review_t>;

		template<typename Table>
		struct get_column : detail::get_review_id<Table>{};

	}review_id{};

	static struct reviewer_id_g
	{
		using _is_column = std::true_type;
		using _is_table  = std::false_type;
		using _is_unique = std::true_type;

		using tag_category  = ::tagsql::core::context_dependent_tag;
		using tables = ::foam::meta::typelist<schema::review_t>;

		template<typename Table>
		struct get_column : detail::get_reviewer_id<Table>{};

	}reviewer_id{};

	static struct title_g
	{
		using _is_column = std::true_type;
		using _is_table  = std::false_type;
		using _is_unique = std::true_type;

		using tag_category  = ::tagsql::core::context_dependent_tag;
		using tables = ::foam::meta::typelist<schema::book_t>;

		template<typename Table>
		struct get_column : detail::get_title<Table>{};

	}title{};

}}}} //snawaz # db # fest # generic_tags
