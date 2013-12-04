

#pragma once


#include <tagsql/developement/keys.h++>
#include <tagsql/developement/meta_table_base.h++>


namespace tagsql { namespace developement { namespace metaspace
{

    template<>
    struct meta_table<tagsql::developement::schema::book> : public meta_table_base<tagsql::developement::schema::book>
    {
        using  sequence_type = ::foam::meta::genseq_t<5>;
        using  primary_key   = tagsql::developement::book_tag::primary_key;
        using  foreign_keys  = tagsql::developement::book_tag::foreign_keys;
        using  columns_tuple = tagsql::developement::book_tag::all;
        static std::string name() {  return "book"; }
        static sequence_type index_sequence() { return {}; }
        static constexpr std::size_t size() { return 5; }
        static columns_tuple columns() { return {}; }
    };

    template<>
    struct meta_table<tagsql::developement::schema::author> : public meta_table_base<tagsql::developement::schema::author>
    {
        using  sequence_type = ::foam::meta::genseq_t<5>;
        using  primary_key   = tagsql::developement::author_tag::primary_key;
        using  foreign_keys  = tagsql::developement::author_tag::foreign_keys;
        using  columns_tuple = tagsql::developement::author_tag::all;
        static std::string name() {  return "author"; }
        static sequence_type index_sequence() { return {}; }
        static constexpr std::size_t size() { return 5; }
        static columns_tuple columns() { return {}; }
    };

}}} //tagsql # developement # metaspace
