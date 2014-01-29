

#pragma once


#include <tagsql/insert_adaptor.h++>


namespace tagsql { namespace formatting
{

	#if 0
	template<typename Adaptor>
	class formatter<::tagsql::schema::book_t, Adaptor>
	{
		public:
			using value_type = ::tagsql::schema::book_t;
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
	class formatter<::tagsql::schema::author_t, Adaptor>
	{
		public:
			using value_type = ::tagsql::schema::author_t;
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
	#endif

}} //tagsql # formatting
