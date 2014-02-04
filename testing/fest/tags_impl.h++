

#pragma once


#include <tagsql/support/supported_types.h++>
#include <string>
#include <tagsql/core/tag_category.h++>
#include <tagsql/core/meta_column.h++>

namespace snawaz { namespace db { namespace fest
{
	namespace schema
	{
		struct author_t;
		struct book_t;
		struct review_t;

	} //schema

	namespace author_tag
	{
		/*
		* + column_name    = author_id
		* + column_type    = long int <--- bigint
		* + table_name     = author
		* + primary_key    = true
		* + foreign_key    = false
		* + nullable       = false
		* + server_default = true
		*/
		struct author_id_t : ::tagsql::metaspace::meta_column_t<schema::author_t,long int,false,true>
		{
			using base = ::tagsql::metaspace::meta_column_t<schema::author_t,long int,false,true>;
			using sql_data_type = ::tagsql::support::types::single<::tagsql::support::types::bigint_t>;
			using tag_category  = ::tagsql::core::context_independent_tag;

			template<typename T>
			struct named_member
			{
				T author_id;
			};

			author_id_t() : base("author_id"){}

			static char const* type_name() { return "long int"; }
		};

		/*
		* + column_name    = name
		* + column_type    = std::string <--- name [63]
		* + table_name     = author
		* + primary_key    = false
		* + foreign_key    = false
		* + nullable       = true
		* + server_default = false
		*/
		struct name_t : ::tagsql::metaspace::meta_column_t<schema::author_t,std::string,true,false>
		{
			using base = ::tagsql::metaspace::meta_column_t<schema::author_t,std::string,true,false>;
			using sql_data_type = ::tagsql::support::types::multi<::tagsql::support::types::character_t,63>;
			using tag_category  = ::tagsql::core::context_independent_tag;

			template<typename T>
			struct named_member
			{
				T name;
			};

			name_t() : base("name"){}

			static char const* type_name() { return "std::string"; }
		};

		/*
		* + column_name    = age
		* + column_type    = short <--- smallint
		* + table_name     = author
		* + primary_key    = false
		* + foreign_key    = false
		* + nullable       = true
		* + server_default = false
		*/
		struct age_t : ::tagsql::metaspace::meta_column_t<schema::author_t,short,true,false>
		{
			using base = ::tagsql::metaspace::meta_column_t<schema::author_t,short,true,false>;
			using sql_data_type = ::tagsql::support::types::single<::tagsql::support::types::smallint_t>;
			using tag_category  = ::tagsql::core::context_independent_tag;

			template<typename T>
			struct named_member
			{
				T age;
			};

			age_t() : base("age"){}

			static char const* type_name() { return "short"; }

			template<typename TableList>
			static std::string repr(TableList) { return "author.age"; }
		};

		/*
		* + column_name    = modified
		* + column_type    = std::string <--- timestamp with time zone
		* + table_name     = author
		* + primary_key    = false
		* + foreign_key    = false
		* + nullable       = true
		* + server_default = false
		*/
		struct modified_t : ::tagsql::metaspace::meta_column_t<schema::author_t,std::string,true,false>
		{
			using base = ::tagsql::metaspace::meta_column_t<schema::author_t,std::string,true,false>;
			using sql_data_type = ::tagsql::support::types::single<::tagsql::support::types::timestamp_t>;
			using tag_category  = ::tagsql::core::context_independent_tag;

			template<typename T>
			struct named_member
			{
				T modified;
			};

			modified_t() : base("modified"){}

			static char const* type_name() { return "std::string"; }
		};

		/*
		* + column_name    = created
		* + column_type    = std::string <--- timestamp with time zone
		* + table_name     = author
		* + primary_key    = false
		* + foreign_key    = false
		* + nullable       = true
		* + server_default = false
		*/
		struct created_t : ::tagsql::metaspace::meta_column_t<schema::author_t,std::string,true,false>
		{
			using base = ::tagsql::metaspace::meta_column_t<schema::author_t,std::string,true,false>;
			using sql_data_type = ::tagsql::support::types::single<::tagsql::support::types::timestamp_t>;
			using tag_category  = ::tagsql::core::context_independent_tag;

			template<typename T>
			struct named_member
			{
				T created;
			};

			created_t() : base("created"){}

			static char const* type_name() { return "std::string"; }
		};

		/*
		* + column_name    = author
		* + column_type    = std::string <--- name [63]
		* + table_name     = author
		* + primary_key    = false
		* + foreign_key    = false
		* + nullable       = true
		* + server_default = false
		*/
		struct author_t : ::tagsql::metaspace::meta_column_t<schema::author_t,std::string,true,false>
		{
			using base = ::tagsql::metaspace::meta_column_t<schema::author_t,std::string,true,false>;
			using sql_data_type = ::tagsql::support::types::multi<::tagsql::support::types::character_t,63>;
			using tag_category  = ::tagsql::core::context_independent_tag;

			template<typename T>
			struct named_member
			{
				T author;
			};

			author_t() : base("author"){}

			static char const* type_name() { return "std::string"; }
		};


	} //author_tag

	namespace book_tag
	{
		/*
		* + column_name    = book_id
		* + column_type    = long int <--- bigint
		* + table_name     = book
		* + primary_key    = true
		* + foreign_key    = false
		* + nullable       = false
		* + server_default = true
		*/
		struct book_id_t : ::tagsql::metaspace::meta_column_t<schema::book_t,long int,false,true>
		{
			using base = ::tagsql::metaspace::meta_column_t<schema::book_t,long int,false,true>;
			using sql_data_type = ::tagsql::support::types::single<::tagsql::support::types::bigint_t>;
			using tag_category  = ::tagsql::core::context_independent_tag;

			template<typename T>
			struct named_member
			{
				T book_id;
			};

			book_id_t() : base("book_id"){}

			static char const* type_name() { return "long int"; }
		};

		/*
		* + column_name    = title
		* + column_type    = std::string <--- character varying [128]
		* + table_name     = book
		* + primary_key    = false
		* + foreign_key    = false
		* + nullable       = true
		* + server_default = false
		*/
		struct title_t : ::tagsql::metaspace::meta_column_t<schema::book_t,std::string,true,false>
		{
			using base = ::tagsql::metaspace::meta_column_t<schema::book_t,std::string,true,false>;
			using sql_data_type = ::tagsql::support::types::multi<::tagsql::support::types::character_varying_t,128>;
			using tag_category  = ::tagsql::core::context_independent_tag;

			template<typename T>
			struct named_member
			{
				T title;
			};

			title_t() : base("title"){}

			static char const* type_name() { return "std::string"; }
		};

		/*
		* + column_name    = author_id
		* + column_type    = long int <--- bigint
		* + table_name     = book
		* + primary_key    = false
		* + foreign_key    = true
		* + nullable       = true
		* + server_default = false
		*/
		struct author_id_t : ::tagsql::metaspace::meta_column_t<schema::book_t,long int,true,false>
		{
			using base = ::tagsql::metaspace::meta_column_t<schema::book_t,long int,true,false>;
			using sql_data_type = ::tagsql::support::types::single<::tagsql::support::types::bigint_t>;
			using tag_category  = ::tagsql::core::context_independent_tag;

			template<typename T>
			struct named_member
			{
				T author_id;
			};

			author_id_t() : base("author_id"){}

			static char const* type_name() { return "long int"; }
		};

		/*
		* + column_name    = modified
		* + column_type    = std::string <--- timestamp with time zone
		* + table_name     = book
		* + primary_key    = false
		* + foreign_key    = false
		* + nullable       = true
		* + server_default = false
		*/
		struct modified_t : ::tagsql::metaspace::meta_column_t<schema::book_t,std::string,true,false>
		{
			using base = ::tagsql::metaspace::meta_column_t<schema::book_t,std::string,true,false>;
			using sql_data_type = ::tagsql::support::types::single<::tagsql::support::types::timestamp_t>;
			using tag_category  = ::tagsql::core::context_independent_tag;

			template<typename T>
			struct named_member
			{
				T modified;
			};

			modified_t() : base("modified"){}

			static char const* type_name() { return "std::string"; }
		};

		/*
		* + column_name    = created
		* + column_type    = std::string <--- timestamp with time zone
		* + table_name     = book
		* + primary_key    = false
		* + foreign_key    = false
		* + nullable       = true
		* + server_default = false
		*/
		struct created_t : ::tagsql::metaspace::meta_column_t<schema::book_t,std::string,true,false>
		{
			using base = ::tagsql::metaspace::meta_column_t<schema::book_t,std::string,true,false>;
			using sql_data_type = ::tagsql::support::types::single<::tagsql::support::types::timestamp_t>;
			using tag_category  = ::tagsql::core::context_independent_tag;

			template<typename T>
			struct named_member
			{
				T created;
			};

			created_t() : base("created"){}

			static char const* type_name() { return "std::string"; }
		};

		/*
		* + column_name    = author
		* + column_type    = std::string <--- character varying [50]
		* + table_name     = book
		* + primary_key    = false
		* + foreign_key    = false
		* + nullable       = true
		* + server_default = false
		*/
		struct author_t : ::tagsql::metaspace::meta_column_t<schema::book_t,std::string,true,false>
		{
			using base = ::tagsql::metaspace::meta_column_t<schema::book_t,std::string,true,false>;
			using sql_data_type = ::tagsql::support::types::multi<::tagsql::support::types::character_varying_t,50>;
			using tag_category  = ::tagsql::core::context_independent_tag;

			template<typename T>
			struct named_member
			{
				T author;
			};

			author_t() : base("author"){}

			static char const* type_name() { return "std::string"; }
		};


	} //book_tag

	namespace review_tag
	{
		/*
		* + column_name    = review_id
		* + column_type    = long int <--- bigint
		* + table_name     = review
		* + primary_key    = true
		* + foreign_key    = false
		* + nullable       = false
		* + server_default = true
		*/
		struct review_id_t : ::tagsql::metaspace::meta_column_t<schema::review_t,long int,false,true>
		{
			using base = ::tagsql::metaspace::meta_column_t<schema::review_t,long int,false,true>;
			using sql_data_type = ::tagsql::support::types::single<::tagsql::support::types::bigint_t>;
			using tag_category  = ::tagsql::core::context_independent_tag;

			template<typename T>
			struct named_member
			{
				T review_id;
			};

			review_id_t() : base("review_id"){}

			static char const* type_name() { return "long int"; }
		};

		/*
		* + column_name    = book_id
		* + column_type    = long int <--- bigint
		* + table_name     = review
		* + primary_key    = false
		* + foreign_key    = true
		* + nullable       = true
		* + server_default = false
		*/
		struct book_id_t : ::tagsql::metaspace::meta_column_t<schema::review_t,long int,true,false>
		{
			using base = ::tagsql::metaspace::meta_column_t<schema::review_t,long int,true,false>;
			using sql_data_type = ::tagsql::support::types::single<::tagsql::support::types::bigint_t>;
			using tag_category  = ::tagsql::core::context_independent_tag;

			template<typename T>
			struct named_member
			{
				T book_id;
			};

			book_id_t() : base("book_id"){}

			static char const* type_name() { return "long int"; }
		};

		/*
		* + column_name    = reviewer_id
		* + column_type    = long int <--- bigint
		* + table_name     = review
		* + primary_key    = false
		* + foreign_key    = true
		* + nullable       = true
		* + server_default = false
		*/
		struct reviewer_id_t : ::tagsql::metaspace::meta_column_t<schema::review_t,long int,true,false>
		{
			using base = ::tagsql::metaspace::meta_column_t<schema::review_t,long int,true,false>;
			using sql_data_type = ::tagsql::support::types::single<::tagsql::support::types::bigint_t>;
			using tag_category  = ::tagsql::core::context_independent_tag;

			template<typename T>
			struct named_member
			{
				T reviewer_id;
			};

			reviewer_id_t() : base("reviewer_id"){}

			static char const* type_name() { return "long int"; }
		};

		/*
		* + column_name    = comment
		* + column_type    = std::string <--- character varying [2048]
		* + table_name     = review
		* + primary_key    = false
		* + foreign_key    = false
		* + nullable       = true
		* + server_default = false
		*/
		struct comment_t : ::tagsql::metaspace::meta_column_t<schema::review_t,std::string,true,false>
		{
			using base = ::tagsql::metaspace::meta_column_t<schema::review_t,std::string,true,false>;
			using sql_data_type = ::tagsql::support::types::multi<::tagsql::support::types::character_varying_t,2048>;
			using tag_category  = ::tagsql::core::context_independent_tag;

			template<typename T>
			struct named_member
			{
				T comment;
			};

			comment_t() : base("comment"){}

			static char const* type_name() { return "std::string"; }
		};


	} //review_tag

}}} //snawaz # db # fest
