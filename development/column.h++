


#pragma once


#include <string>

//#include <tagsql/development/tags.h++>
//#include <tagsql/development/meta_table.h++>

#include <foam/strlib/format.h>
#include <foam/meta/typelist.h++>
#include <foam/optional.h>

#include <pqxx/pqxx>


namespace tagsql { namespace development
{
    struct parse_exception : std::runtime_error
    {
        using std::runtime_error::runtime_error;
    };
   
	template<typename ...Tags>
	struct named_tuple;

    template<typename Tag>
    class column : public Tag::sql_data_type::template operators<Tag>
    {
        public:
            using tag_type   = Tag;
            using table_type = typename Tag::table_type;
            using value_type = typename Tag::column_type;
		
			using _is_column = std::true_type;
			using _is_tag = std::false_type;
    
            static const bool is_nullable    = tag_type::is_nullable; 
            static const bool server_default = tag_type::server_default; 
    
            column() {}
    
            column(value_type value) : _data(std::move(value)) {}
    
            template<typename U, typename Unused=typename std::enable_if<!::foam::meta::bare::is_same<U,pqxx::field>::value>::type >
            column(U && value) 
            {
                static_assert(std::is_constructible<value_type, U>::value, "column::value_type cannot be constructed from the argument");
                set(std::forward<U>(value));
            }
    
            column(pqxx::field const & field) 
            { 
                try
                {
                    if ( !field.is_null() )
                        set(field.as<value_type>());
                }
                catch(std::exception const & e)
                {
                    throw parse_exception(foam::strlib::format("'{0}' cannot be converted into column '{1}' which is of type {2}. Original exception is : {3}.",
                                                                        field.c_str(), column_name(true), Tag::type_name(), e.what() ));
                }
    
            }
    
            //it is not copy-assignment!
            column& operator = (value_type const & value)
            {
                return set(value);
            }
    
            //it is not copy-assignment either!
            template<typename U>
            column& operator = (U && value)
            {
                static_assert(std::is_constructible<value_type, U>::value, "column::value_type cannot be constructed from the argument");
                return set(std::forward<U>(value));
            }
    
            //it is not copy-assignment!
            column& operator = (std::nullptr_t)
            {
                return set(::foam::none);
            }
   
			//value access using method => T v = item.value;
            value_type const & value() const { return *_data; }
            value_type & value()  { return *_data; }

			//value access using function-style call => T v = item();
            value_type const & operator()() const { return value(); }
            value_type & operator()()  { return value(); }
			
			//value access by implicit conversoon => T v = item;
			//operator value_type const & () const { return value(); }
			//operator value_type& () { return value(); }

            bool is_null() const 
            { 
                return _data == ::foam::none; 
            }
    
            bool null_has_been_set_explicitly() const
            {
                return _null_has_been_set;
            }
    
            static std::string column_name(bool qualify) 
            {
                return "TODO" ;//(qualify ? metaspace::meta_table<table_type>::name() + "."  : "") + tag_type().column_name;
            }
    
        private:
			template<typename ...Tags>
			friend class named_tuple;

            template<typename U>
            column& set(U && value)
            {
			 	using type = typename std::remove_cv<typename std::remove_reference<U>::type>::type;
				set_internal(std::forward<U>(value), static_cast<type*>(0));
				return *this;
			}
            template<typename U>
            void set_internal(U && value, pqxx::field const *)
			{
                try
                {
                    if ( !value.is_null() )
                        set(value.template as<value_type>());
                }
                catch(std::exception const & e)
                {
                    throw parse_exception(foam::strlib::format("'{0}' cannot be converted into column '{1}' which is of type {2}. Original exception is : {3}.",
			                                                            value.c_str(), column_name(true), Tag::type_name(), e.what() ));
				}
			}
            template<typename U>
            void set_internal(U && other, column<Tag> const *)
			{
				if ( not other.is_null() )
                	set(other.value());
				else if ( other.null_has_been_set_explicitly() )
                	set(::foam::none);
			}
            template<typename U>
            void set_internal(U && value, ... )
			{
                _data = std::forward<U>(value);
                _null_has_been_set =  (_data == ::foam::none);
            }
        private:
            ::foam::optional<value_type> _data;
            bool                         _null_has_been_set = false;
    };
   
	namespace ostream_detail
	{
		template<typename Tag>
		auto print(std::ostream & out, column<Tag> const &c, int) -> decltype(out << c(), out)
		{
			return out << c();
		}
		
		template<typename Tag>
		auto print(std::ostream & out, column<Tag> const &c, ... ) -> decltype(out)
		{
			return out << Tag::type_name() << "<" <<  static_cast<void const*>(&c) << ">";
		}
	}

	template<typename Tag>
    std::ostream & operator <<(std::ostream & out, column<Tag> const & c)
	{
		if ( c.is_null() ) 
			return out << "NULL";
		return ostream_detail::print(out, c, 0);
    }

}} //tagsql # development
