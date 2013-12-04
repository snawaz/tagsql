

#pragma once


#include <foam/strlib/format.h>
#include <string>
#include <tagsql/developement/column.h++>
#include <tagsql/developement/tags.h++>


namespace tagsql { namespace developement { namespace schema
{

    //This definition of schema for table 'book'.
    namespace txzi1 = ::tagsql::developement::book_tag;
    struct book
    {
        column <txzi1::created_t>                     created;                 //std::string	-	-	-
        column <txzi1::modified_t>                    modified;                //std::string	-	-	-
        column <txzi1::author_id_t>                   author_id;               //long int	-	foreign_key	-
        column <txzi1::title_t>                       title;                   //std::string	-	-	-
        column <txzi1::book_id_t>                     book_id;                 //long int	non-nullable	primary_key	server_default
    };

    //This definition of schema for table 'author'.
    namespace txzi2 = ::tagsql::developement::author_tag;
    struct author
    {
        column <txzi2::created_t>                     created;                 //std::string	-	-	-
        column <txzi2::modified_t>                    modified;                //std::string	-	-	-
        column <txzi2::age_t>                         age;                     //short	-	-	-
        column <txzi2::name_t>                        name;                    //std::string	-	-	-
        column <txzi2::author_id_t>                   author_id;               //long int	non-nullable	primary_key	server_default
    };

    std::ostream& operator<<(std::ostream& out, book const & item)
    {
        return ::foam::strlib::print(out, "{0},{1},{2},{3},{4}", item.created,item.modified,item.author_id,item.title,item.book_id);
    }

    std::ostream& operator<<(std::ostream& out, author const & item)
    {
        return ::foam::strlib::print(out, "{0},{1},{2},{3},{4}", item.created,item.modified,item.age,item.name,item.author_id);
    }

}}} //tagsql # developement # schema
