

#pragma once


#include <string>
#include <tagsql/meta_column.h++>
#include <tagsql/support/supported_types.h++>


namespace tagsql
{
    namespace schema
    {
        struct book_t;
        struct author_t;
		struct review_t;
    
	} //schema

    namespace book_tag
    {
        /*
        * + column_name    = created
        * + column_type    = std::string <--- timestamp with time zone
        * + table_name     = book
        * + primary_key    = false
        * + foreign_key    = false
        * + nullable       = true
        * + server_default = false
        */
        struct created_t : metaspace::meta_column_t<schema::book_t,std::string,true,false>
        {
            using base = metaspace::meta_column_t<schema::book_t,std::string,true,false>;
			using sql_data_type = support::types::single<support::types::timestamp_t>;

			template<typename T>
			struct named_member
			{
				T created;
			};

//            std::string created;

            created_t() : base("created"){}

//           void set(std::string value) { created = std::move(value); }

//           std::string const & get() const { return created; }

            static char const* type_name() { return "std::string"; }
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
        struct modified_t : metaspace::meta_column_t<schema::book_t,std::string,true,false>
        {
            using base = metaspace::meta_column_t<schema::book_t,std::string,true,false>;
			using sql_data_type = support::types::single<support::types::timestamp_t>;

			template<typename T>
			struct named_member
			{
				T modified;
			};

            //std::string modified;

            modified_t() : base("modified"){}

            //void set(std::string value) { modified = std::move(value); }

            //std::string const & get() const { return modified; }

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
        struct author_id_t : metaspace::meta_column_t<schema::book_t,long int,true,false>
        {
            using base = metaspace::meta_column_t<schema::book_t,long int,true,false>;
			using sql_data_type = support::types::single<support::types::bigserial_t>;

			template<typename T>
			struct named_member
			{
				T author_id;
			};

            //long int author_id;

            author_id_t() : base("author_id"){}

            //void set(long int value) { author_id = std::move(value); }

            //long int const & get() const { return author_id; }

            static char const* type_name() { return "long int"; }
        };

        /*
        * + column_name    = title
        * + column_type    = std::string <--- character varying
        * + table_name     = book
        * + primary_key    = false
        * + foreign_key    = false
        * + nullable       = true
        * + server_default = false
        */
        struct title_t : metaspace::meta_column_t<schema::book_t,std::string,true,false>
        {
            using base = metaspace::meta_column_t<schema::book_t,std::string,true,false>;
			using sql_data_type = support::types::multi<support::types::character_varying_t, 128>;

			template<typename T>
			struct named_member
			{
				T title;
			};

            //std::string title;

            title_t() : base("title"){}

            //void set(std::string value) { title = std::move(value); }

            //std::string const & get() const { return title; }

            static char const* type_name() { return "std::string"; }
        };

        /*
        * + column_name    = book_id
        * + column_type    = long int <--- bigint
        * + table_name     = book
        * + primary_key    = true
        * + foreign_key    = false
        * + nullable       = false
        * + server_default = true
        */
        struct book_id_t : metaspace::meta_column_t<schema::book_t,long int,false,true>
        {
            using base = metaspace::meta_column_t<schema::book_t,long int,false,true>;
			using sql_data_type = support::types::single<support::types::bigserial_t>;

			template<typename T>
			struct named_member
			{
				T book_id;
			};

            //long int book_id;

            book_id_t() : base("book_id"){}

            //void set(long int value) { book_id = std::move(value); }

            //long int const & get() const { return book_id; }

            static char const* type_name() { return "long int"; }
        };


    } //book_tag

    namespace author_tag
    {
        /*
        * + column_name    = created
        * + column_type    = std::string <--- timestamp with time zone
        * + table_name     = author
        * + primary_key    = false
        * + foreign_key    = false
        * + nullable       = true
        * + server_default = false
        */
        struct created_t : metaspace::meta_column_t<schema::author_t,std::string,true,false>
        {
            using base = metaspace::meta_column_t<schema::author_t,std::string,true,false>;
			using sql_data_type = support::types::single<support::types::timestamp_t>;

			template<typename T>
			struct named_member
			{
				T created;
			};

            //std::string created;

            created_t() : base("created"){}

            //void set(std::string value) { created = std::move(value); }

            //std::string const & get() const { return created; }

            static char const* type_name() { return "std::string"; }
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
        struct modified_t : metaspace::meta_column_t<schema::author_t,std::string,true,false>
        {
            using base = metaspace::meta_column_t<schema::author_t,std::string,true,false>;
			using sql_data_type = support::types::single<support::types::timestamp_t>;

			template<typename T>
			struct named_member
			{
				T modified;
			};

            //std::string modified;

            modified_t() : base("modified"){}

            //void set(std::string value) { modified = std::move(value); }

            //std::string const & get() const { return modified; }

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
        struct age_t : metaspace::meta_column_t<schema::author_t,short,true,false>
        {
            using base = metaspace::meta_column_t<schema::author_t,short,true,false>;
			using sql_data_type = support::types::single<support::types::smallint_t>;
			
			template<typename T>
			struct named_member
			{
				T age;
			};

            //short age;

            age_t() : base("age"){}

            //void set(short value) { age = std::move(value); }

            //short const & get() const { return age; }

            static char const* type_name() { return "short"; }
        };

        /*
        * + column_name    = name
        * + column_type    = std::string <--- name
        * + table_name     = author
        * + primary_key    = false
        * + foreign_key    = false
        * + nullable       = true
        * + server_default = false
        */
        struct name_t : metaspace::meta_column_t<schema::author_t,std::string,true,false>
        {
            using base = metaspace::meta_column_t<schema::author_t,std::string,true,false>;
			using sql_data_type = support::types::multi<support::types::character_t, 64>;

			template<typename T>
			struct named_member
			{
				T name;
			};

            //std::string name;

            name_t() : base("name"){}

            //void set(std::string value) { name = std::move(value); }

            //std::string const & get() const { return name; }

            static char const* type_name() { return "std::string"; }
        };

        /*
        * + column_name    = author_id
        * + column_type    = long int <--- bigint
        * + table_name     = author
        * + primary_key    = true
        * + foreign_key    = false
        * + nullable       = false
        * + server_default = true
        */
        struct author_id_t : metaspace::meta_column_t<schema::author_t,long int,false,true>
        {
            using base = metaspace::meta_column_t<schema::author_t,long int,false,true>;
			using sql_data_type = support::types::single<support::types::bigserial_t>;

			template<typename T>
			struct named_member
			{
				T author_id;
			};

            //long int author_id;

            author_id_t() : base("author_id"){}

            //void set(long int value) { author_id = std::move(value); }

            //long int const & get() const { return author_id; }

            static char const* type_name() { return "long int"; }
        };


    } //author_tag
	
	namespace review_tag
    {
        struct review_id_t : metaspace::meta_column_t<schema::review_t, long int, false, true>
		{
            using base = metaspace::meta_column_t<schema::review_t,long int,false,true>;
			using sql_data_type = support::types::single<support::types::bigserial_t>;

			template<typename T>
			struct named_member
			{
				T review_id;
			};

            //long int review_id;

            review_id_t() : base("review_id"){}

            //void set(long int value) { review_id = std::move(value); }

            //long int const & get() const { return review_id; }

            static char const* type_name() { return "long int"; }
		};

        struct reviewer_id_t  :  metaspace::meta_column_t<schema::review_t, long int, false, false>
		{
            using base = metaspace::meta_column_t<schema::review_t,long int,false,false>;
			using sql_data_type = support::types::single<support::types::bigint_t>;

			template<typename T>
			struct named_member
			{
				T reviewer_id;
			};

            //long int reviewer_id;

            reviewer_id_t() : base("reviewer_id"){}

            //void set(long int value) { reviewer_id = std::move(value); }

            //long int const & get() const { return reviewer_id; }

            static char const* type_name() { return "long int"; }
		};

        struct book_id_t :  metaspace::meta_column_t<schema::review_t, long int, false, false>
		{
            using base = metaspace::meta_column_t<schema::review_t,long int,false, false>;
			using sql_data_type = support::types::single<support::types::bigint_t>;
			
			template<typename T>
			struct named_member
			{
				T book_id;
			};

            //long int book_id;

            book_id_t() : base("book_id"){}

            //void set(long int value) { book_id = std::move(value); }

            //long int const & get() const { return book_id; }

            static char const* type_name() { return "long int"; }
		};

		struct mystring 
		{
			mystring() = default;
			mystring(char const *) {}
			mystring(std::string const &) {}

			//friend std::ostream& operator<<(std::ostream & out, mystring const& item) { return out << "mystr::addr<" << &item << ">"; }
		};

        struct comment_t :  metaspace::meta_column_t<schema::review_t, mystring, true, false>
		{
            using base = metaspace::meta_column_t<schema::review_t,mystring,true,false>;
			using sql_data_type = support::types::multi<support::types::character_t, 2048>;

			template<typename T>
			struct named_member
			{
				T comment;
			};

			//mystring comment;

            comment_t() : base("comment"){}

            //void set(std::string value) { comment = std::move(value); }

			//std::string const & get() const { return comment; }

            static char const* type_name() { return "mystring"; }
		};
    }

} //tagsql

namespace pqxx
{
	template<>
	struct string_traits<tagsql::review_tag::mystring>
	{
		using mystring = tagsql::review_tag::mystring;

		static void from_string(char const *, mystring &) {} 
		static mystring null() { return {}; }
	};
}
