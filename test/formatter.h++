

#pragma once


#include <tagsql/developement/insert_adaptor.h++>


namespace tagsql { namespace developement { namespace formatting
{

    template<typename Adaptor>
    class formatter<::tagsql::developement::schema::book, Adaptor>
    {
        public:
            using value_type = ::tagsql::developement::schema::book;
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
            Adaptor                                       _adaptor;                
            output_type                                   _output;                 
    };

    template<typename Adaptor>
    class formatter<::tagsql::developement::schema::author, Adaptor>
    {
        public:
            using value_type = ::tagsql::developement::schema::author;
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
            Adaptor                                       _adaptor;                
            output_type                                   _output;                 
    };

}}} //tagsql # developement # formatting
