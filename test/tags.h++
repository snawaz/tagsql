

#pragma once


#include <string>
#include <tagsql/developement/tags_impl.h++>


namespace tagsql { namespace developement
{

    /*
    * + The definition of tags for table 'book.'
    * + The user can use these tags in select-clause, where-clause, and on-clause, as well as when accessing values from select results.
    * + 
    * + Examples,
    * + 
    * + 	using namespace = tagsql::developement::schema; //bring all the table names from 'schema' namespace.
    * + 	namespace bt = tagsql::developement::book_tag; //make easy-to-use alias for specific 'tag' namespace.
    * + 
    * + 	auto items = context.select( bt::created, bt::modified, bt::author_id, bt::title )
    * + 	                    .from<book>()
    * + 	                    .where()
    * + 
    * + Hope that helps. :-)
    */
    namespace book_tag
    {
        static const created_t                        created;                 //std::string	-	-	-
        static const modified_t                       modified;                //std::string	-	-	-
        static const author_id_t                      author_id;               //long int	-	foreign_key	-
        static const title_t                          title;                   //std::string	-	-	-
        static const book_id_t                        book_id;                 //long int	non-nullable	primary_key	server_default

    } //book_tag

    /*
    * + The definition of tags for table 'author.'
    * + The user can use these tags in select-clause, where-clause, and on-clause, as well as when accessing values from select results.
    * + 
    * + Examples,
    * + 
    * + 	using namespace = tagsql::developement::schema; //bring all the table names from 'schema' namespace.
    * + 	namespace at = tagsql::developement::author_tag; //make easy-to-use alias for specific 'tag' namespace.
    * + 
    * + 	auto items = context.select( at::created, at::modified, at::age, at::name )
    * + 	                    .from<author>()
    * + 	                    .where()
    * + 
    * + Hope that helps. :-)
    */
    namespace author_tag
    {
        static const created_t                        created;                 //std::string	-	-	-
        static const modified_t                       modified;                //std::string	-	-	-
        static const age_t                            age;                     //short	-	-	-
        static const name_t                           name;                    //std::string	-	-	-
        static const author_id_t                      author_id;               //long int	non-nullable	primary_key	server_default

    } //author_tag

}} //tagsql # developement
