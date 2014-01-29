

#pragma once


#include <tagsql/test/named_tuple.h++>
#include <tagsql/test/tags.h++>


namespace tagsql { namespace test { namespace schema
{

	//This definition of schema for table 'author'.
	namespace txzi1 = ::tagsql::test::author_tag;
	using author_t = named_tuple<
	                              txzi1::author_id_t,       //long int            	non-nullable	primary_key 	server_default
	                              txzi1::name_t,            //std::string         	-           	-           	-             
	                              txzi1::age_t,             //short               	-           	-           	-             
	                              txzi1::modified_t,        //std::string         	-           	-           	-             
	                              txzi1::created_t          //std::string         	-           	-           	-             	
	                            >;

	//This definition of schema for table 'book'.
	namespace txzi2 = ::tagsql::test::book_tag;
	using book_t = named_tuple<
	                            txzi2::book_id_t,         //long int            	non-nullable	primary_key 	server_default
	                            txzi2::title_t,           //std::string         	-           	-           	-             
	                            txzi2::author_id_t,       //long int            	-           	foreign_key 	-             
	                            txzi2::modified_t,        //std::string         	-           	-           	-             
	                            txzi2::created_t          //std::string         	-           	-           	-             	
	                          >;

	//This definition of schema for table 'review'.
	namespace txzi3 = ::tagsql::test::review_tag;
	using review_t = named_tuple<
	                              txzi3::review_id_t,       //long int            	non-nullable	primary_key 	server_default
	                              txzi3::book_id_t,         //long int            	-           	foreign_key 	-             
	                              txzi3::reviewer_id_t,     //long int            	-           	foreign_key 	-             
	                              txzi3::comment_t          //std::string         	-           	-           	-             	
	                            >;

}}} //tagsql # test # schema
