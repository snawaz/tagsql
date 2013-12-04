

#include <iostream>
#include "data_context.h++"
#include "table.h++"

template<typename Item>
std::ostream& operator<<(std::ostream & out, tagsql::test::deferred_range<Item> const & items)
{
	for(auto const & item : items)
		out << item << "\n";
		//int i = item;
	return out;
}

void test_select(tagsql::test::data_context & dc)
{
	using namespace tagsql::test::schema;

	auto items = dc.select().from<author>();
	//int x = items;
	std::cout << items << std::endl;
}

void test_insert(tagsql::test::data_context & dc)
{
	using namespace tagsql::test::schema;
	author a {};
	a.name = "Saurabh";
	a.age = 20;
	dc.insert(a);
}


int main()
{
    try
    {
		tagsql::test::data_context dc("test", "localhost", 5432, "snawaz", "itsnotme");
		test_select(dc);
		test_insert(dc);
    }
    catch(std::exception const & e)
    {
		std::cout << "exception in main() : " << e.what() << std::endl;
    }
}
