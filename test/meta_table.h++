

#pragma once


#include <tagsql/test/keys.h++>
#include <tagsql/test/meta_table_base.h++>


namespace tagsql { namespace test { namespace metaspace
{

	template<>
	struct meta_table<tagsql::test::schema::author> : public meta_table_base<tagsql::test::schema::author>
	{
		using  sequence_type = ::foam::meta::genseq_t<5>;
		using  primary_key   = tagsql::test::author_tag::primary_key;
		using  foreign_keys  = tagsql::test::author_tag::foreign_keys;
		using  columns_tuple = tagsql::test::author_tag::all;
		static std::string name() {  return "author"; }
		static sequence_type index_sequence() { return {}; }
		static constexpr std::size_t size() { return 5; }
		static columns_tuple columns() { return {}; }
	};

	template<>
	struct meta_table<tagsql::test::schema::book> : public meta_table_base<tagsql::test::schema::book>
	{
		using  sequence_type = ::foam::meta::genseq_t<5>;
		using  primary_key   = tagsql::test::book_tag::primary_key;
		using  foreign_keys  = tagsql::test::book_tag::foreign_keys;
		using  columns_tuple = tagsql::test::book_tag::all;
		static std::string name() {  return "book"; }
		static sequence_type index_sequence() { return {}; }
		static constexpr std::size_t size() { return 5; }
		static columns_tuple columns() { return {}; }
	};

	template<>
	struct meta_table<tagsql::test::schema::review> : public meta_table_base<tagsql::test::schema::review>
	{
		using  sequence_type = ::foam::meta::genseq_t<4>;
		using  primary_key   = tagsql::test::review_tag::primary_key;
		using  foreign_keys  = tagsql::test::review_tag::foreign_keys;
		using  columns_tuple = tagsql::test::review_tag::all;
		static std::string name() {  return "review"; }
		static sequence_type index_sequence() { return {}; }
		static constexpr std::size_t size() { return 4; }
		static columns_tuple columns() { return {}; }
	};

}}} //tagsql # test # metaspace
