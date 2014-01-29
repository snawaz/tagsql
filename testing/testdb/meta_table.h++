

#pragma once


#include <tagsql/meta_table_base.h++>

#include <testing/testdb/keys.h++>

namespace tagsql { namespace metaspace
{

    template<>
    struct meta_table<testing::schema::book_t> : public meta_table_base<testing::schema::book_t>
    {
        using  sequence_type = ::foam::meta::genseq_t<5>;
        using  primary_key   = testing::book_tag::primary_key;
        using  foreign_keys  = testing::book_tag::foreign_keys;
        using  columns_tuple = testing::book_tag::all;
        static std::string name() {  return "book"; }
        static sequence_type index_sequence() { return {}; }
        static constexpr std::size_t size() { return 5; }
        static columns_tuple columns() { return {}; }
    };

    template<>
    struct meta_table<testing::schema::author_t> : public meta_table_base<testing::schema::author_t>
    {
        using  sequence_type = ::foam::meta::genseq_t<5>;
        using  primary_key   = testing::author_tag::primary_key;
        using  foreign_keys  = testing::author_tag::foreign_keys;
        using  columns_tuple = testing::author_tag::all;
        static std::string name() {  return "author"; }
        static sequence_type index_sequence() { return {}; }
        static constexpr std::size_t size() { return 5; }
        static columns_tuple columns() { return {}; }
    };

    template<>
    struct meta_table<testing::schema::review_t> : public meta_table_base<testing::schema::review_t>
    {
        using  sequence_type = ::foam::meta::genseq_t<4>;
        using  primary_key   = testing::review_tag::primary_key;
        using  foreign_keys  = testing::review_tag::foreign_keys;
        using  columns_tuple = testing::review_tag::all;
        static std::string name() {  return "review"; }
        static sequence_type index_sequence() { return {}; }
        static constexpr std::size_t size() { return 4; }
        static columns_tuple columns() { return {}; }
	};

}} //tagsql # metaspace
