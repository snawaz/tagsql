

#pragma once


#include <tagsql/test/insert_adaptor.h++>


namespace tagsql { namespace test { namespace formatting
{

	template<typename Adaptor>
	class formatter<::tagsql::test::schema::author, Adaptor>
	{
		public:
			using value_type = ::tagsql::test::schema::author;
			using output_type = typename Adaptor::value_type;

			formatter(value_type const & value)
			{
				_adaptor.write(value.author_id, _output);
				_adaptor.write(value.name, _output);
				_adaptor.write(value.age, _output);
				_adaptor.write(value.modified, _output);
				_adaptor.write(value.created, _output);
			}
			output_type const & output() const { return _output; }
		private:
			Adaptor                                       _adaptor;                
			output_type                                   _output;                 
	};

	template<typename Adaptor>
	class formatter<::tagsql::test::schema::book, Adaptor>
	{
		public:
			using value_type = ::tagsql::test::schema::book;
			using output_type = typename Adaptor::value_type;

			formatter(value_type const & value)
			{
				_adaptor.write(value.book_id, _output);
				_adaptor.write(value.title, _output);
				_adaptor.write(value.author_id, _output);
				_adaptor.write(value.modified, _output);
				_adaptor.write(value.created, _output);
			}
			output_type const & output() const { return _output; }
		private:
			Adaptor                                       _adaptor;                
			output_type                                   _output;                 
	};

	template<typename Adaptor>
	class formatter<::tagsql::test::schema::review, Adaptor>
	{
		public:
			using value_type = ::tagsql::test::schema::review;
			using output_type = typename Adaptor::value_type;

			formatter(value_type const & value)
			{
				_adaptor.write(value.review_id, _output);
				_adaptor.write(value.book_id, _output);
				_adaptor.write(value.reviewer_id, _output);
				_adaptor.write(value.comment, _output);
			}
			output_type const & output() const { return _output; }
		private:
			Adaptor                                       _adaptor;                
			output_type                                   _output;                 
	};

}}} //tagsql # test # formatting
