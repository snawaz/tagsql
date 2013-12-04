
#include <iostream>
#include <sstream>
#include <string>
#include <foam/optional.h>
#include <stdexcept>

template<typename Tag>
class column : public Tag::sql_data_type::template operators<Tag>
{
	public:
		using value_type = typename Tag::column_type;

		template<typename T>
		column& operator=(T && data)
		{
			_data = data;
			return *this;
		}
		value_type value() const
		{
			return _data.value() ;// == ::foam::none ? std::runtime_error("null value") : _data;
		}
	private:
		::foam::optional<value_type> _data;
};

namespace op
{
	struct eq
	{
		template<typename T, typename U>
		static auto apply(T && t, U && u) -> decltype( t == u )
		{
			return t == u;
		}
		
		static char const *db_operator()
		{
			return "=";
		}
	};
	
	struct like
	{
		template<typename T, typename U>
		static auto apply(T && t, U && u) -> decltype( t == u )
		{
			return t == u;
		}
		
		static char const *db_operator()
		{
			return "LIKE";
		}
	};
}

template<typename Tag>
struct column_expression
{
	using tag = Tag;
	using Operators = typename Tag::sql_data_type::single_type:: template operators<Tag>;

	Operators & _this;

	auto operator()() const
	{
		return static_cast<column<tag>&>(_this).value();
	}

	std::string repr() const
	{
		return tag::column_name();
	}
};

template<typename T>
struct literal_expression
{
	T _value;

	T operator()() const
	{
		return _value;
	}

	std::string repr() const
	{
		return _repr(_value);
	}
	
	std::string _repr(std::string const &) const { return _value; }

	std::string _repr(...) const { return std::to_string(_value); }
};


template<typename Left, typename Right, typename Operator>
struct logical_binary_expression
{
	Left _left;
	Right _right;

	bool operator()() const
	{
		static bool value = Operator::apply(_left(), _right()); //ensure one time computation!
		return value;
	}

	std::string repr() const
	{
		return _left.repr() + " " +  Operator::db_operator() + " " + _right.repr();
	}

	operator bool() const
	{
		return this->operator()();
	}

	operator std::string() const
	{
		return repr();
	}
};

namespace support
{
	template<typename DbType>
	struct single 
	{
		using single_type = DbType;

		template<typename Tag>
		struct operators : DbType::template operators<Tag> {};
	};
	
	template<typename DbType, std::size_t Size>
	struct multi
	{ 
		using single_type = DbType;

		static constexpr std::size_t size = Size; 
		
		template<typename Tag>
		struct operators : DbType::template operators<Tag> { };
	};

	struct character_t
	{
		template<typename Tag>
		struct operators
		{
			using literal_type = typename Tag::column_type;

			auto operator==(literal_type value) -> logical_binary_expression<column_expression<Tag>, literal_expression<literal_type>, op::eq>
			{
				return { *this, value}; 
			}
		};
	};

	template<std::size_t Size>
	struct multi<character_t, Size>
	{ 
		using single_type = character_t;

		static constexpr std::size_t size = Size; 
		
		template<typename Tag>
		struct operators : character_t::operators<Tag> 
		{ 
			using literal_type = typename Tag::column_type;

			auto like(literal_type pattern) -> logical_binary_expression<column_expression<Tag>, literal_expression<literal_type>, op::like>
			{
				return { *this, pattern };
			}
		};
	};

	struct integer_t
	{
		template<typename Tag>
		struct operators
		{
			using literal_type = typename Tag::column_type;

			auto operator==(literal_type value) -> logical_binary_expression<column_expression<Tag>, literal_expression<literal_type>, op::eq>
			{
				return { *this, value }; 
			}
		};
	};
}

struct name_t
{
	using column_type = std::string;
	using sql_data_type = support::multi<support::character_t, 64>;

	static char const *column_name() { return "name"; }

};
struct age_t
{
	using column_type = int;
	using sql_data_type = support::single<support::integer_t>;
	
	static char const *column_name() { return "age"; }
};

int main()
{
	column<name_t> name;
	column<age_t>  age;

	{
		name = "Naw";
		auto x = name.like("Naw%");
		bool b = x;
		std::string s = x;
		std::cout << std::boolalpha << b << std::endl;
		std::cout << s << std::endl;
	}

	{
		age = 30;
		auto x = age == 30;

		if ( x ) std::cout << "TRUE" << std::endl;
		if ( !x ) std::cout << "FALSE" << std::endl;

		age = 20;
		if ( !x ) std::cout << "BUT FALSE" << std::endl;

		bool b = x;
		std::string s = x;
		std::cout << std::boolalpha << b << std::endl;
		std::cout << s << std::endl;
	}
}
