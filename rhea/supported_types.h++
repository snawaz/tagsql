

#pragma once

#include <map>
#include <string>
#include <tagsql/common/exceptions.h++>

namespace tagsql { namespace rhea
{
	static const std::map<std::string, std::string> supported_types
	{
		//numeric types
		{"smallint", "smallint_t"},
		{"integer", "integer_t"},
		{"bigint", "bigint_t"},
		{"decimal", "decimal_t"},
		{"numeric", "numeric_t"},
		{"real", "real_t"},
		{"double precision", "double_precision_t"},
		{"serial", "serial_t"},
		{"bigserial", "bigserial_t"},

		//character types
		{"character", "character_t"},
		{"character varying", "character_varying_t"},
		{"text", "text_t"},

		//date and time types
		{"timestamp", "timestamp_t"},
		{"date", "date_t"},
		{"time", "time_t"},
		{"interval", "interval_t"},
	};

	struct sql_type_info
	{
		std::string name;
		std::size_t size;
	};

	static const std::map<std::string, sql_type_info> alias_types
	{
		{"name", {"character", 63}},
		{"char", {"character", 0}},
		{"nvarchar", {"character varying", 0}},
	};

	static sql_type_info get_type_info(std::string const & name)
	{
		auto const & key = (name == "timestamp with time zone" ? "timestamp" : name);
		auto it = supported_types.find(key);
		if ( it == supported_types.end() )
		{
			auto it = alias_types.find(key);
			if ( it == alias_types.end() )
				throw unsupported_type(name + " is not supported type.");
			return {get_type_info(it->second.name).name, it->second.size};
		}
		return {it->second, 0};
	}
}}
