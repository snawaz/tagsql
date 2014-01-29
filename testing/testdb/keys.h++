

#pragma once


#include <testing/testdb/tags_impl.h++>


namespace testing
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

    namespace review_tag
    {
        using primary_key  = review_id_t; //assuming at most one column can be primary_key.

        using foreign_keys = std::tuple<std::tuple<reviewer_id_t, author_tag::author_id_t>, std::tuple<book_id_t, book_tag::book_id_t>>;

        using all = std::tuple<review_id_t, reviewer_id_t, book_id_t, comment_t>;

        template<typename Tag>
        std::string qualify(Tag const & tag) { return "review." + tag.column_name; }
	}

} //tagsql
