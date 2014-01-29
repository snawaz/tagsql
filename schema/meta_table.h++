

#pragma once


#include <tagsql/schema/keys.h++>
#include <tagsql/meta_table_base.h++>


namespace tagsql { namespace metaspace
{

    template<>
    struct meta_table<tagsql::schema::book_t> : public meta_table_base<tagsql::schema::book_t>
    {
        using  sequence_type = ::foam::meta::genseq_t<5>;
        using  primary_key   = tagsql::book_tag::primary_key;
        using  foreign_keys  = tagsql::book_tag::foreign_keys;
        using  columns_tuple = tagsql::book_tag::all;
        static std::string name() {  return "book"; }
        static sequence_type index_sequence() { return {}; }
        static constexpr std::size_t size() { return 5; }
        static columns_tuple columns() { return {}; }
    };

    template<>
    struct meta_table<tagsql::schema::author_t> : public meta_table_base<tagsql::schema::author_t>
    {
        using  sequence_type = ::foam::meta::genseq_t<5>;
        using  primary_key   = tagsql::author_tag::primary_key;
        using  foreign_keys  = tagsql::author_tag::foreign_keys;
        using  columns_tuple = tagsql::author_tag::all;
        static std::string name() {  return "author"; }
        static sequence_type index_sequence() { return {}; }
        static constexpr std::size_t size() { return 5; }
        static columns_tuple columns() { return {}; }
    };

    template<>
    struct meta_table<tagsql::schema::review_t> : public meta_table_base<tagsql::schema::review_t>
    {
        using  sequence_type = ::foam::meta::genseq_t<4>;
        using  primary_key   = tagsql::review_tag::primary_key;
        using  foreign_keys  = tagsql::review_tag::foreign_keys;
        using  columns_tuple = tagsql::review_tag::all;
        static std::string name() {  return "review"; }
        static sequence_type index_sequence() { return {}; }
        static constexpr std::size_t size() { return 4; }
        static columns_tuple columns() { return {}; }
	};

}} //tagsql # metaspace
