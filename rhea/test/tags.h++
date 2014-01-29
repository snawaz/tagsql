

#pragma once


#include <string>
#include <tagsql/test/tags_impl.h++>


namespace tagsql { namespace test
{

	namespace author_tag
	{
		static const author_id_t                      author_id;               //long int	non-nullable	primary_key	server_default
		static const name_t                           name;                    //std::string	-	-	-
		static const age_t                            age;                     //short	-	-	-
		static const modified_t                       modified;                //std::string	-	-	-
		static const created_t                        created;                 //std::string	-	-	-

	} //author_tag

	namespace book_tag
	{
		static const book_id_t                        book_id;                 //long int	non-nullable	primary_key	server_default
		static const title_t                          title;                   //std::string	-	-	-
		static const author_id_t                      author_id;               //long int	-	foreign_key	-
		static const modified_t                       modified;                //std::string	-	-	-
		static const created_t                        created;                 //std::string	-	-	-

	} //book_tag

	namespace review_tag
	{
		static const review_id_t                      review_id;               //long int	non-nullable	primary_key	server_default
		static const book_id_t                        book_id;                 //long int	-	foreign_key	-
		static const reviewer_id_t                    reviewer_id;             //long int	-	foreign_key	-
		static const comment_t                        comment;                 //std::string	-	-	-

	} //review_tag

}} //tagsql # test
