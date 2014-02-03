

#include <iostream>

#include <testing/fest/fest.h++>

const snawaz::db::fest::schema::author_t _author{};
const snawaz::db::fest::schema::book_t   _book{};
const snawaz::db::fest::schema::review_t _review{};

using namespace snawaz::db::fest;
using namespace snawaz::db::fest::generic_tags;

template<typename NamedTuple>
void fff(NamedTuple item)
{
	std::cout << item << std::endl;
}

void f(tagsql::named_tuple<author_tag::name_t, author_tag::age_t> item)
{
	std::cout << "f() => " << item.name << "," << item.age << std::endl;
	std::cout << "f() => " << item.at<0>() <<"," << item[author.age] << std::endl;
}

void h(tagsql::named_tuple<author_tag::age_t, author_tag::name_t> item)
{
	std::cout << "h() => " << item.age << "," << item.name << std::endl;
	std::cout << "h() => " << item << std::endl;
}

//void g(named_tuple<author_tag::name_t, author_tag::created_t> item)
void g(tagsql::named_tuple<author_tag::name_t> item)
{
	std::cout << "g() => " << item.name << std::endl;
	std::cout << "g() => " << item << std::endl;
}

void test_generic_tags(tagsql::data_context & dc)
{
	//namespace t=snawaz::db::fest::generic_tags;

	std::cout << dc.select(created, book.title).from(book) << std::endl;
	std::cout << dc.select(created).from(author) << std::endl;

	std::cout << dc.select(book.title).from(book) << std::endl;
	//std::cout << (author.author_id == 10) << std::endl;
	//std::cout << dc.select(book.title, author.name).from(book).join(author).on(author.author_id == 10) << std::endl;
}

#if 0
void test_select(tagsql::data_context & dc)
{
	using namespace tagsql;
	using namespace snawaz::db::fest::schema;
	namespace at = snawaz::db::fest::author_tag;
	namespace bt = snawaz::db::fest::book_tag;
	namespace rt = snawaz::db::fest::review_tag;

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
void test_join(tagsql::data_context & dc)
{
	using namespace snawaz::db::fest::schema;
	namespace at = snawaz::db::fest::author_tag;
	namespace bt = snawaz::db::fest::book_tag;
	namespace rt = snawaz::db::fest::review_tag;

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

void test_insert(tagsql::data_context & dc)
{
	using namespace snawaz::db::fest::schema;
	author_t a {};
	a.name = "Saurabh Khan";
	int x = (a.age = 20000);
	//std::cout << x << std::endl;
	//dc.insert(a);

	//tagsql::transaction xtras(dc);

	//db syntax, more rows at once
	//dc.insert_into(author)(_author.name, _author.age).values("C++", 11).values("C#", 12).values("Java", 13); //.commit();

#if 0
	author_t author;
	auto items = dc.select(author.name, author.age).from(author);
	for(auto const & item  : items)
	{
		std::cout << "DEMO : " << item.name << ", " << item.age << std::endl;
	}
#endif

	using types = ::foam::meta::typelist<author_t, review_t>; 
	//using types = ::foam::meta::typelist<book_t, review_t>; 
	//using types = ::foam::meta::typelist<author_t, book_t, review_t>; 
	auto e1 = age == age; //std::string("Sarfaraz Nawaz");
	auto e2 = author == author; //std::string("Sarfaraz Nawaz");
	auto e3 = age == author; //std::string("Sarfaraz Nawaz");
	std::cout << e1.repr(types()) << std::endl;
	std::cout << e2.repr(types()) << std::endl;
	std::cout << e3.repr(types()) << std::endl;

	//db syntax
	//dc.insert_into(book).columns(_book.title, _book.author_id).values("C++", 10).commit();             

#if 1 //NOT_DONE_YET	
	//improved syntax (inspired from UPDATE command)
	//std::cout << "before insert_into() a = " << a << std::endl;
	//dc.insert_into(author).set(a.age = 10);            
	//dc.insert_into(author).set(a.name = "C++", a.age = 10).set(a.age = 100, a.name = "Java");
	//std::cout << "after insert_into() a = " << a << std::endl;
	//dc.insert_into(author).set(_author.name = "C++", _author.age = 10).set(...).set( ... so on ... );            
#endif 
	//book_t b{};	
	//dc.update(author).set(a.age = 110).where(b.author_id == 1);
	//dc.update(author).set(a.age = 110).where(a.author_id == 2);

	//xtrans.commit(); OR xtrans.rollback(); or whatever makes sense
}

int main()
{
    try
    {
		tagsql::data_context dc("test", "localhost", 5432, "snawaz", "itsnotme");
		//test_generic_tags(dc);
#if 0		
		test_select(dc);
		test_join(dc);
#endif		
		test_insert(dc);
    }
    catch(std::exception const & e)
    {
		std::cout << "exception in main().\n" << e.what() << std::endl;
    }
}
