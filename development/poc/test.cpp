
#include <iostream>
#include "../column.h++"
#include "../tags.h++"

int main()
{
	using tagsql::development::column;
	using namespace tagsql::development::author_tag;

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
