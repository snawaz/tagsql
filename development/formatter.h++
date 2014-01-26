

#pragma once


#include <tagsql/development/insert_adaptor.h++>


namespace tagsql { namespace development { namespace formatting
{

	template<typename Adaptor>
	class formatter<::tagsql::development::schema::book_t, Adaptor>
	{
		public:
			using value_type = ::tagsql::development::schema::book_t;
			using output_type = typename Adaptor::value_type;
			formatter(value_type const & value)
			{
				_adaptor.write(value.created, _output);
				_adaptor.write(value.modified, _output);
				_adaptor.write(value.author_id, _output);
				_adaptor.write(value.title, _output);
				_adaptor.write(value.book_id, _output);
			}
			output_type const & output() const { return _output; }
		private:
			Adaptor        _adaptor;                
			output_type    _output;
	};

	template<typename Adaptor>
	class formatter<::tagsql::development::schema::author_t, Adaptor>
	{
		public:
			using value_type = ::tagsql::development::schema::author_t;
			using output_type = typename Adaptor::value_type;

			formatter(value_type const & value)
			{
				_adaptor.write(value.created, _output);
				_adaptor.write(value.modified, _output);
				_adaptor.write(value.age, _output);
				_adaptor.write(value.name, _output);
				_adaptor.write(value.author_id, _output);
			}
			output_type const & output() const { return _output; }
		private:
			Adaptor        _adaptor;                
			output_type    _output;
	};

}}} //tagsql # development # formatting
