

#pragma once


#include <foam/meta/typelist.h++>
#include <string>
#include <tagsql/development/column.h++>
#include <tagsql/development/tags.h++>
#include <tagsql/development/operators.h++>

namespace tagsql { namespace development { namespace support
{
	namespace types
	{
		//Numeric types
		//http://www.postgresql.org/docs/9.1/static/datatype-numeric.html
		struct smallint_t {};
		struct integer_t {};
		struct bigint_t {};
		struct decimal_t {};
		struct numeric_t {};
		struct real_t {};
		struct double_precision_t {};
		struct serial_t {};
		struct bigserial_t {};

		using numeric_types = ::foam::meta::typelist<smallint_t, integer_t, bigint_t, decimal_t, numeric_t, real_t, double_precision_t, serial_t, bigserial_t>;

		//Character types
		//http://www.postgresql.org/docs/9.1/static/datatype-character.html
		struct character_varying_t {};
		struct character_t {};
		struct text_t {};

		using nvarchar_t = character_varying_t;
		using char_t   = character_t;
		using character_types = ::foam::meta::typelist<character_varying_t, character_t, text_t>;

		//Date/Time types
		//http://www.postgresql.org/docs/9.1/static/datatype-datetime.html
		struct timestamp_t {};
		struct date_t {};
		struct time_t {};
		struct interval_t {};
	
		template<typename DbType>
		struct single 
		{
			template<typename Tag>
			struct operators : op::eq_t<Tag> {};
		};

		template<typename DbType, std::size_t Size>
		struct multi
		{
			static constexpr std::size_t size = Size;

			template<typename Tag>
			struct operators : op::eq_t<Tag> {};
		};
	
		template<std::size_t Size>
		struct multi<types::character_t, Size>
		{
			static constexpr std::size_t size = Size;

			template<typename Tag>
			struct operators : op::eq_t<Tag>, op::like_t<Tag> {};
		};
	}
	

}}} //tagsql # development # schema
