

#pragma once


#include <testing/fest/tags.h++>
#include <tagsql/named_tuple.h++>


namespace snawaz { namespace db { namespace fest { namespace schema
{

	//This definition of schema for table 'author'.
	namespace txzi1 = ::snawaz::db::fest::author_tag;
	using _author_base_t = ::tagsql::named_tuple<
	                                              txzi1::author_id_t,       //long int            	non-nullable	primary_key 	server_default
	                                              txzi1::name_t,            //std::string         	-           	-           	-             
	                                              txzi1::age_t,             //short               	-           	-           	-             
	                                              txzi1::modified_t,        //std::string         	-           	-           	-             
	                                              txzi1::created_t          //std::string         	-           	-           	-             	
	                                            >;
	struct author_t : public _author_base_t
	{
		using table = author_t;
		using  _author_base_t::_author_base_t;
	};

	//This definition of schema for table 'book'.
	namespace txzi2 = ::snawaz::db::fest::book_tag;
	using _book_base_t = ::tagsql::named_tuple<
	                                            txzi2::book_id_t,         //long int            	non-nullable	primary_key 	server_default
	                                            txzi2::title_t,           //std::string         	-           	-           	-             
	                                            txzi2::author_id_t,       //long int            	-           	foreign_key 	-             
	                                            txzi2::modified_t,        //std::string         	-           	-           	-             
	                                            txzi2::created_t          //std::string         	-           	-           	-             	
	                                          >;
	struct book_t : public _book_base_t
	{
		using table = book_t;
		using  _book_base_t::_book_base_t;
	};

	//This definition of schema for table 'review'.
	namespace txzi3 = ::snawaz::db::fest::review_tag;
	using _review_base_t = ::tagsql::named_tuple<
	                                              txzi3::review_id_t,       //long int            	non-nullable	primary_key 	server_default
	                                              txzi3::book_id_t,         //long int            	-           	foreign_key 	-             
	                                              txzi3::reviewer_id_t,     //long int            	-           	foreign_key 	-             
	                                              txzi3::comment_t          //std::string         	-           	-           	-             	
	                                            >;
	struct review_t : public _review_base_t
	{
		using table = review_t;
		using  _review_base_t::_review_base_t;
	};

}}}} //snawaz # db # fest # schema
