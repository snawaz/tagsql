

#pragma once


#include <string>
#include <tagsql/developement/meta_column.h++>


namespace tagsql { namespace developement
{
    namespace schema
    {
        struct book;
        struct author;

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
        struct created_t : metaspace::meta_column_t<schema::book,std::string,true,false>
        {
            using base = metaspace::meta_column_t<schema::book,std::string,true,false>;

            std::string created;

            created_t() : base("created"){}

            void set(std::string value) { created = std::move(value); }

            std::string const & get() const { return created; }

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
        struct modified_t : metaspace::meta_column_t<schema::book,std::string,true,false>
        {
            using base = metaspace::meta_column_t<schema::book,std::string,true,false>;

            std::string modified;

            modified_t() : base("modified"){}

            void set(std::string value) { modified = std::move(value); }

            std::string const & get() const { return modified; }

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

            long int author_id;

            author_id_t() : base("author_id"){}

            void set(long int value) { author_id = std::move(value); }

            long int const & get() const { return author_id; }

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
        struct title_t : metaspace::meta_column_t<schema::book,std::string,true,false>
        {
            using base = metaspace::meta_column_t<schema::book,std::string,true,false>;

            std::string title;

            title_t() : base("title"){}

            void set(std::string value) { title = std::move(value); }

            std::string const & get() const { return title; }

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
        struct book_id_t : metaspace::meta_column_t<schema::book,long int,false,true>
        {
            using base = metaspace::meta_column_t<schema::book,long int,false,true>;

            long int book_id;

            book_id_t() : base("book_id"){}

            void set(long int value) { book_id = std::move(value); }

            long int const & get() const { return book_id; }

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
        struct created_t : metaspace::meta_column_t<schema::author,std::string,true,false>
        {
            using base = metaspace::meta_column_t<schema::author,std::string,true,false>;

            std::string created;

            created_t() : base("created"){}

            void set(std::string value) { created = std::move(value); }

            std::string const & get() const { return created; }

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
        struct modified_t : metaspace::meta_column_t<schema::author,std::string,true,false>
        {
            using base = metaspace::meta_column_t<schema::author,std::string,true,false>;

            std::string modified;

            modified_t() : base("modified"){}

            void set(std::string value) { modified = std::move(value); }

            std::string const & get() const { return modified; }

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

            short age;

            age_t() : base("age"){}

            void set(short value) { age = std::move(value); }

            short const & get() const { return age; }

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
        struct name_t : metaspace::meta_column_t<schema::author,std::string,true,false>
        {
            using base = metaspace::meta_column_t<schema::author,std::string,true,false>;

            std::string name;

            name_t() : base("name"){}

            void set(std::string value) { name = std::move(value); }

            std::string const & get() const { return name; }

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
        struct author_id_t : metaspace::meta_column_t<schema::author,long int,false,true>
        {
            using base = metaspace::meta_column_t<schema::author,long int,false,true>;

            long int author_id;

            author_id_t() : base("author_id"){}

            void set(long int value) { author_id = std::move(value); }

            long int const & get() const { return author_id; }

            static char const* type_name() { return "long int"; }
        };


    } //author_tag

}} //tagsql # developement
