

#pragma once


#include <testing/fest/table.h++>
#include <testing/fest/keys.h++>
#include <tagsql/core/meta_table_base.h++>


namespace tagsql { namespace metaspace
{

	template<>
	struct meta_table<::snawaz::db::fest::schema::author_t> : public meta_table_base<::snawaz::db::fest::schema::author_t>
	{
		using  sequence_type = ::foam::meta::genseq_t<6>;
		using  primary_key   = ::snawaz::db::fest::author_tag::primary_key;
		using  foreign_keys  = ::snawaz::db::fest::author_tag::foreign_keys;
		using  columns_tuple = ::snawaz::db::fest::author_tag::all;
		static std::string name() {  return "author"; }
		static sequence_type index_sequence() { return {}; }
		static constexpr std::size_t size() { return 6; }
		static columns_tuple columns() { return {}; }
	};

	template<>
	struct meta_table<::snawaz::db::fest::schema::book_t> : public meta_table_base<::snawaz::db::fest::schema::book_t>
	{
		using  sequence_type = ::foam::meta::genseq_t<6>;
		using  primary_key   = ::snawaz::db::fest::book_tag::primary_key;
		using  foreign_keys  = ::snawaz::db::fest::book_tag::foreign_keys;
		using  columns_tuple = ::snawaz::db::fest::book_tag::all;
		static std::string name() {  return "book"; }
		static sequence_type index_sequence() { return {}; }
		static constexpr std::size_t size() { return 6; }
		static columns_tuple columns() { return {}; }
	};

	template<>
	struct meta_table<::snawaz::db::fest::schema::review_t> : public meta_table_base<::snawaz::db::fest::schema::review_t>
	{
		using  sequence_type = ::foam::meta::genseq_t<4>;
		using  primary_key   = ::snawaz::db::fest::review_tag::primary_key;
		using  foreign_keys  = ::snawaz::db::fest::review_tag::foreign_keys;
		using  columns_tuple = ::snawaz::db::fest::review_tag::all;
		static std::string name() {  return "review"; }
		static sequence_type index_sequence() { return {}; }
		static constexpr std::size_t size() { return 4; }
		static columns_tuple columns() { return {}; }
	};

}} //tagsql # metaspace
