

#include <iostream>
#include "data_context.h++"
#include "table.h++"

/*
template<typename Item>
std::ostream& operator<<(std::ostream & out, tagsql::redesign::deferred_range<Item> const & items)
{
	for(auto const & item : items)
		out << item << "\n";
		//int i = item;
	return out;
}
*/

void test_select(tagsql::redesign::data_context & dc)
{
	//using namespace tagsql::redesign::schema;
	using namespace tagsql::redesign::tags;

	auto items = dc.select(author.name, author.age).from(author);
	//int x = items;
	std::cout << items << std::endl;
}

void test_insert(tagsql::redesign::data_context & dc)
{
	using namespace tagsql::redesign::schema;
	author a {};
	a.name = "Saurabh";
	a.age = 20;
	dc.insert(a);
}


int main()
{
    try
    {
		tagsql::redesign::data_context dc("test", "localhost", 5432, "snawaz", "itsnotme");
		test_select(dc);
		//test_insert(dc);
    }
    catch(std::exception const & e)
    {
		std::cout << "exception in main() : " << e.what() << std::endl;
    }
}
