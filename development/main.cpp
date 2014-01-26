

#include <iostream>
#include "data_context.h++"
#include "schema/table.h++"
#include "schema/tags.h++"
#include "schema/universal_tags.h++"

const tagsql::development::schema::author_t author{};
const tagsql::development::schema::book_t book{};
const tagsql::development::schema::review_t review{};

using namespace tagsql::development;

template<typename NamedTuple>
void fff(NamedTuple item)
{
	std::cout << item << std::endl;
}
void f(named_tuple<author_tag::name_t, author_tag::age_t> item)
{
	std::cout << "f() => " << item.name << "," << item.age << std::endl;
	std::cout << "f() => " << item.at<0>() <<"," << item[author.age] << std::endl;
}

void h(named_tuple<author_tag::age_t, author_tag::name_t> item)
{
	std::cout << "h() => " << item.age << "," << item.name << std::endl;
	std::cout << "h() => " << item << std::endl;
}


//void g(named_tuple<author_tag::name_t, author_tag::created_t> item)
void g(named_tuple<author_tag::name_t> item)
{
	std::cout << "g() => " << item.name << std::endl;
	std::cout << "g() => " << item << std::endl;
}

void test_universal_tags(tagsql::development::data_context & dc)
{
	namespace t=tagsql::development::universal_tags;

	std::cout << dc.select(t::created).from(book) << std::endl;
	std::cout << dc.select(t::created).from(author) << std::endl;

	std::cout << dc.select(book.title).from(book) << std::endl;

}
#if 0
void test_select(tagsql::development::data_context & dc)
{
	using namespace tagsql::development;
	using namespace tagsql::development::schema;
	namespace at = tagsql::development::author_tag;
	namespace bt = tagsql::development::book_tag;
	namespace rt = tagsql::development::review_tag;

	for(auto const & item : dc.select().from(author))
	{
		f(item);
		h(item);
		g(item);
		break;
	}

	std::cout << "-----------------------------\n";
	
	auto items = dc.select(at::name, author.age)
				   .from(author)
				   .where(author.name.like("Sha%"));

								//.where(author.age == 30);
	//auto items = dc.select(at::name, at::age, bt::title).from(author).where(author.author_id == 30);
	//auto items = dc.select(at::name, at::age).from(author).where(true); //book.author_id == 30);
	//auto items = dc.select(at::name).from(author);//.where(book.author_id == 30);

	f(*items.begin());
	h(*items.begin());
	g(*items.begin());
	std::cout << std::endl;

	std::cout << items << std::endl;
	
	//std::cout << dc.select().from(review).where(review.book_id == 2).limit(1).where(review.book_id == 2) << std::endl;   //no WHERE
	//std::cout << dc.select().from(review).limit(1).where(review.book_id == 2) << std::endl;                              //no WHERE
	//std::cout << dc.select().from(review).where(review.book_id == 2).limit(1).fetch(1) << std::endl;                     //no LIMIT
	std::cout << dc.select().from(review).cross_join(book).join(author).on(author.author_id == book.author_id) << std::endl; 
	std::cout << dc.select().from(review).where(review.book_id == 2) << std::endl; 
	std::cout << dc.select().from(review).where(review.book_id == 2).order_by(review.review_id) << std::endl; 
	std::cout << dc.select().from(review).where(review.book_id == 2).order_by(review.review_id, sql::desc) << std::endl; 
	std::cout << dc.select().from(review).where(review.book_id == 2).order_by(review.review_id, sql::asc) << std::endl; 
	std::cout << dc.select().from(review).where(review.book_id == 2).limit(sql::all) << std::endl; 
	std::cout << dc.select().from(review).where(review.book_id == 2).limit(1).offset(1) << std::endl; 
	std::cout << dc.select(review.review_id, rt::book_id).from(review).where(review.book_id == 2).offset(1).fetch(1) << std::endl; 

	std::cout << dc.select().from(review).group_by(review.review_id) << std::endl; 

	//std::cout << dc.select().from(book).group_by(book.title) << std::endl; 

	std::cout << dc.select().from(review).where(review.reviewer_id == 2).order_by(review.review_id, sql::asc, review.book_id, sql::desc) << std::endl; 
	std::cout << dc.select().from(review).where(review.reviewer_id == 2).order_by(review.review_id, sql::asc, review.book_id, sql::asc) << std::endl; 
	std::cout << dc.select().from(review).where(review.reviewer_id == 2).order_by(review.review_id, sql::desc, review.book_id, sql::asc) << std::endl; 
	std::cout << dc.select().from(review).where(review.reviewer_id == 2).order_by(-review.review_id, +review.book_id) << std::endl; 

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
	std::cout << items << std::endl;
#else  
	auto items = dc.select() //at::name, bt::title, rt::comment)
				   .from(author)
				   .left_join(book).on(book.author_id == author.author_id)
				   .join(review).on(review.reviewer_id == author.author_id);

	std::cout << items << std::endl;
#endif

}
#endif 

int main()
{
    try
    {
		tagsql::development::data_context dc("test", "localhost", 5432, "snawaz", "itsnotme");
		test_universal_tags(dc);
//		test_select(dc);
//		test_insert(dc);
//		test_join(dc);
    }
    catch(std::exception const & e)
    {
		std::cout << "exception in main().\n" << e.what() << std::endl;
    }
}
