

#pragma once


#include <string>
#include <tagsql/test/meta_column.h++>


namespace tagsql { namespace test
{
	namespace schema
	{
		struct author;
		struct book;
		struct review;

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
		struct author_id_t : metaspace::meta_column_t<schema::author,long int,false,true>
		{
			using base = metaspace::meta_column_t<schema::author,long int,false,true>;
			using sql_data_type = support::types::single<support::types::bigint_t>;

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
		struct name_t : metaspace::meta_column_t<schema::author,std::string,true,false>
		{
			using base = metaspace::meta_column_t<schema::author,std::string,true,false>;
			using sql_data_type = support::types::multi<support::types::character_t,63>;

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
		struct age_t : metaspace::meta_column_t<schema::author,short,true,false>
		{
			using base = metaspace::meta_column_t<schema::author,short,true,false>;
			using sql_data_type = support::types::single<support::types::smallint_t>;

			template<typename T>
			struct named_member
			{
				T age;
			};

			age_t() : base("age"){}

			static char const* type_name() { return "short"; }
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
		struct modified_t : metaspace::meta_column_t<schema::author,std::string,true,false>
		{
			using base = metaspace::meta_column_t<schema::author,std::string,true,false>;
			using sql_data_type = support::types::single<support::types::timestamp_t>;

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
		struct created_t : metaspace::meta_column_t<schema::author,std::string,true,false>
		{
			using base = metaspace::meta_column_t<schema::author,std::string,true,false>;
			using sql_data_type = support::types::single<support::types::timestamp_t>;

			template<typename T>
			struct named_member
			{
				T created;
			};

			created_t() : base("created"){}

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
		struct book_id_t : metaspace::meta_column_t<schema::book,long int,false,true>
		{
			using base = metaspace::meta_column_t<schema::book,long int,false,true>;
			using sql_data_type = support::types::single<support::types::bigint_t>;

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
		struct title_t : metaspace::meta_column_t<schema::book,std::string,true,false>
		{
			using base = metaspace::meta_column_t<schema::book,std::string,true,false>;
			using sql_data_type = support::types::multi<support::types::character_varying_t,128>;

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
		struct author_id_t : metaspace::meta_column_t<schema::book,long int,true,false>
		{
			using base = metaspace::meta_column_t<schema::book,long int,true,false>;
			using sql_data_type = support::types::single<support::types::bigint_t>;

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
		struct modified_t : metaspace::meta_column_t<schema::book,std::string,true,false>
		{
			using base = metaspace::meta_column_t<schema::book,std::string,true,false>;
			using sql_data_type = support::types::single<support::types::timestamp_t>;

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
		struct created_t : metaspace::meta_column_t<schema::book,std::string,true,false>
		{
			using base = metaspace::meta_column_t<schema::book,std::string,true,false>;
			using sql_data_type = support::types::single<support::types::timestamp_t>;

			template<typename T>
			struct named_member
			{
				T created;
			};

			created_t() : base("created"){}

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
		struct review_id_t : metaspace::meta_column_t<schema::review,long int,false,true>
		{
			using base = metaspace::meta_column_t<schema::review,long int,false,true>;
			using sql_data_type = support::types::single<support::types::bigint_t>;

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
		struct book_id_t : metaspace::meta_column_t<schema::review,long int,true,false>
		{
			using base = metaspace::meta_column_t<schema::review,long int,true,false>;
			using sql_data_type = support::types::single<support::types::bigint_t>;

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
		struct reviewer_id_t : metaspace::meta_column_t<schema::review,long int,true,false>
		{
			using base = metaspace::meta_column_t<schema::review,long int,true,false>;
			using sql_data_type = support::types::single<support::types::bigint_t>;

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
		struct comment_t : metaspace::meta_column_t<schema::review,std::string,true,false>
		{
			using base = metaspace::meta_column_t<schema::review,std::string,true,false>;
			using sql_data_type = support::types::multi<support::types::character_varying_t,2048>;

			template<typename T>
			struct named_member
			{
				T comment;
			};

			comment_t() : base("comment"){}

			static char const* type_name() { return "std::string"; }
		};


	} //review_tag

}} //tagsql # test
