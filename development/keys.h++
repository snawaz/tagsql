

#pragma once


#include <tagsql/development/tags_impl.h++>


namespace tagsql { namespace development
{

    namespace book_tag
    {
        using primary_key  = book_id_t; //assuming at most one column can be primary_key.

        using foreign_keys = std::tuple<std::tuple<author_id_t,author_tag::author_id_t>>;

        using all = std::tuple<created_t,modified_t,author_id_t,title_t,book_id_t>;

        template<typename Tag>
        std::string qualify(Tag const & tag) { return "book." + tag.column_name; }

    } //book_tag

    namespace author_tag
    {
        using primary_key  = author_id_t; //assuming at most one column can be primary_key.

        using foreign_keys = std::tuple<>;

        using all = std::tuple<created_t,modified_t,age_t,name_t,author_id_t>;

        template<typename Tag>
        std::string qualify(Tag const & tag) { return "author." + tag.column_name; }

    } //author_tag

}} //tagsql # development
