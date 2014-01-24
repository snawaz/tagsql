

#pragma once




namespace tagsql { namespace test
{
	struct null{}

	template<typename T>
	struct is_null : std::is_same<T,null> {};


}} //tagsql # test
