

#pragma once


#include <tagsql/development/schema/keys.h++>
#include <tagsql/development/meta_table_base.h++>


namespace tagsql { namespace development { namespace metaspace
{

    template<>
    struct meta_table<tagsql::development::schema::book_t> : public meta_table_base<tagsql::development::schema::book_t>
    {
        using  sequence_type = ::foam::meta::genseq_t<5>;
        using  primary_key   = tagsql::development::book_tag::primary_key;
        using  foreign_keys  = tagsql::development::book_tag::foreign_keys;
        using  columns_tuple = tagsql::development::book_tag::all;
        static std::string name() {  return "book"; }
        static sequence_type index_sequence() { return {}; }
        static constexpr std::size_t size() { return 5; }
        static columns_tuple columns() { return {}; }
    };

    template<>
    struct meta_table<tagsql::development::schema::author_t> : public meta_table_base<tagsql::development::schema::author_t>
    {
        using  sequence_type = ::foam::meta::genseq_t<5>;
        using  primary_key   = tagsql::development::author_tag::primary_key;
        using  foreign_keys  = tagsql::development::author_tag::foreign_keys;
        using  columns_tuple = tagsql::development::author_tag::all;
        static std::string name() {  return "author"; }
        static sequence_type index_sequence() { return {}; }
        static constexpr std::size_t size() { return 5; }
        static columns_tuple columns() { return {}; }
    };

    template<>
    struct meta_table<tagsql::development::schema::review_t> : public meta_table_base<tagsql::development::schema::review_t>
    {
        using  sequence_type = ::foam::meta::genseq_t<4>;
        using  primary_key   = tagsql::development::review_tag::primary_key;
        using  foreign_keys  = tagsql::development::review_tag::foreign_keys;
        using  columns_tuple = tagsql::development::review_tag::all;
        static std::string name() {  return "review"; }
        static sequence_type index_sequence() { return {}; }
        static constexpr std::size_t size() { return 4; }
        static columns_tuple columns() { return {}; }
	};

}}} //tagsql # development # metaspace
