
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

template<typename Tag>
struct column_expression
{
	column<Tag> & _column;

	auto eval() const -> typename Tag::column_type
	{
		return _column.value();
	}

	std::string repr() const
	{
		return Tag::column_name();
	}
};

template<typename T>
struct literal_expression
{
	T _value;

	T eval() const
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

struct and_t
{
	template<typename T, typename U>
	static auto apply(T && t, U && u) -> decltype ( t && u )
	{
		return t && u;
	}
	
	static constexpr char const * name = "AND";
};

template<typename Left, typename Right, typename Operator>
struct binary_expression
{
	Left _left;
	Right _right;
	using eval_type = decltype(Operator::apply(std::declval<Left>().eval(), std::declval<Right>().eval()));

	auto eval() const -> eval_type
	{
		static bool value = Operator::apply(_left.eval(), _right.eval()); //ensure one time computation!
		return value;
	}

	std::string repr() const
	{
		static std::string value = _left.repr() + " " +  Operator::name + " " + _right.repr();
		return value;
	}

	operator bool() const { return eval(); }

	operator std::string() const { return repr(); }

	template<typename OtherExpr>
	auto operator && (OtherExpr const & other) -> binary_expression<binary_expression<Left, Right, Operator>, OtherExpr, and_t>
	{
		return { *this, other };
	}
};

namespace op
{
	template<typename Tag>
	struct eq_t
	{
		template<typename T, typename U>
		static auto apply(T && t, U && u) -> decltype( t == u )
		{
			return t == u;
		}
		
		static constexpr char const * name = "=";
		
		using literal_type = typename Tag::column_type;

		auto operator==(literal_type value) -> binary_expression<column_expression<Tag>, literal_expression<literal_type>, eq_t<Tag>>
		{
			return { static_cast<column<Tag>&>(*this), value }; 
		}
	};
	
	template<typename Tag>
	struct like_t
	{
		template<typename T, typename U>
		static auto apply(T && t, U && u) -> decltype( t == u )
		{
			return t == u;
		}
		
		static constexpr char const * name = "LIKE";
		
		using literal_type = typename Tag::column_type;

		auto like(literal_type pattern) -> binary_expression<column_expression<Tag>, literal_expression<literal_type>, like_t<Tag>>
		{
			return { static_cast<column<Tag>&>(*this), pattern }; 
		}
	};
}

namespace support
{
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
		struct operators : op::eq_t<Tag> { };
	};

	struct character_t
	{
		template<typename Tag>
		struct operators : op::eq_t<Tag> {};
	};

	template<std::size_t Size>
	struct multi<character_t, Size>
	{ 
		static constexpr std::size_t size = Size; 

	    template<typename Tag>
		struct operators : op::eq_t<Tag>, op::like_t<Tag> {};
	};

	struct integer_t
	{
		template<typename Tag>
		struct operators : op::eq_t<Tag>  {};
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
		std::string s = x and name == "Sarfaraz";
		std::cout << std::boolalpha << b << std::endl;
		std::cout << s << std::endl;
	}
}
