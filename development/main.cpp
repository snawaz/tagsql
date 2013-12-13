

#include <iostream>
#include "data_context.h++"
#include "table.h++"
#include "tags.h++"

template<typename Item>
std::ostream& operator<<(std::ostream & out, tagsql::development::deferred_range<Item> const & items)
{
	out << "--------\n";
	for(auto const & item : items)
		out << item << "\n";
	return out;
}

//named_tuple printing
template<typename NamedTuple, int ...N>
std::ostream & print_named_tuple(std::ostream & out, NamedTuple const & t, ::foam::meta::seq<N...> )
{
    int x[] = {(out << (N? ", ":"{") << t.template at<N>(), 0)...};
    (void)x;
    return out << "}"; 
}

template<typename ... Tags>
std::ostream & operator << (std::ostream & out, tagsql::development::named_tuple<Tags...> const & t)
{
    return print_named_tuple(out, t, ::foam::meta::make_seq<Tags...>());
}

const tagsql::development::schema::author_t author{};
const tagsql::development::schema::book_t book{};
const tagsql::development::schema::review_t review{};

using namespace tagsql::development;

void f(named_tuple<author_tag::name_t, author_tag::age_t> item)
{
	//std::cout << "f() => " << item.name << "," << item.age << std::endl;
	std::cout << "f() => " << item.at<0>() <<"," << item[author_tag::age] << std::endl;
}

void h(named_tuple<author_tag::age_t, author_tag::name_t> item)
{
	//std::cout << "h() => " << item.age << "," << item.name << std::endl;
	std::cout << "h() => " << item << std::endl;
}

//void g(named_tuple<author_tag::name_t, author_tag::created_t> item)
void g(named_tuple<author_tag::name_t> item)
{
	//std::cout << "g() => " << item.name << std::endl;
	std::cout << "g() => " << item << std::endl;
}


void test_select(tagsql::development::data_context & dc)
{
	using namespace tagsql::development::schema;
	namespace at = tagsql::development::author_tag;
	namespace bt = tagsql::development::book_tag;

	auto items = dc.select(at::name, at::age)
				   .from(author)
				   .where(author.name.like("Sha%"));

	f(*items.begin());
	h(*items.begin());
	g(*items.begin());
				   //.where(author.age == 30);
	//auto items = dc.select(at::name, at::age, bt::title).from(author).where(author.author_id == 30);
	//auto items = dc.select(at::name, at::age).from(author).where(true); //book.author_id == 30);
	//auto items = dc.select(at::name).from(author);//.where(book.author_id == 30);
	std::cout << items << std::endl;
}

void test_insert(tagsql::development::data_context & dc)
{
	using namespace tagsql::development::schema;
	author_t a {};
	a.name = "Saurabh";
	a.age = 20;
	dc.insert(a);
}
void test_join(tagsql::development::data_context & dc)
{
	using namespace tagsql::development::schema;
	namespace at = tagsql::development::author_tag;
	namespace bt = tagsql::development::book_tag;
	namespace rt = tagsql::development::review_tag;

#if 0
	auto items = dc.select(at::name, bt::title)
				   .from(author)
				   .join(book).on(bt::author_id == at::author_id);
#elif 0 
	//auto items = dc.select(at::name, at::age)
	auto items = dc.select(at::name, bt::title, rt::comment)
				   .from(author)
				   .left_join(book).on(book.author_id == author.author_id)
				   .inner_join(review).on(review.reviewer_id == author.author_id);

#endif
#if 0
	sd::cout << items << std::endl;
#endif 

}

int main()
{
    try
    {
		tagsql::development::data_context dc("test", "localhost", 5432, "snawaz", "itsnotme");
		test_select(dc);
//		test_insert(dc);
		test_join(dc);
    }
    catch(std::exception const & e)
    {
		std::cout << "exception in main() : " << e.what() << std::endl;
    }
}
