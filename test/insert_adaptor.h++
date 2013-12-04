





#pragma once


#include <type_traits>
#include <string>
#include <map>
#include <tuple>

#include <foam/strlib/format.h>
#include <tagsql/developement/table.h++>
#include <tagsql/developement/meta_table.h++>


namespace tagsql { namespace developement { namespace formatting
{
    
    namespace detail
    {
        template<typename Value>
        std::string decorate(Value && value, std::true_type)
        {
        return std::to_string(value);
        }
        template<typename Value>
        std::string decorate(Value && value, std::false_type)
        {
        return "'" + foam::strlib::format("{0}", std::forward<Value>(value)) + "'";
        }
    }
    
    template<typename Value>
    std::string decorate(Value && value)
    {
        return detail::decorate(value, typename std::is_arithmetic<typename std::remove_reference<Value>::type>::type());
    }
    
    template<typename T, typename Adaptor>
    class formatter;
    
    struct constraint_violation : public std::runtime_error
    {
        using std::runtime_error::runtime_error;
    };
    
    class sql_insert_adaptor
    {
        public:
        using value_type = std::tuple<std::vector<std::string>, std::vector<std::string>>;
    
        template<typename Column>
        void write(Column const & value, value_type & output)
        {
            using tagsql::developement::metaspace::meta_table;
    
            if ( value.is_null() )
            { 
            if ( value.is_nullable )
            {
                if ( value.null_has_been_set_explicitly() )
                {
                //to ensure that NULL is inserted even if value.server_default is TRUE
                std::get<0>(output).push_back(typename Column::tag_type().column_name);
                std::get<1>(output).push_back("NULL");
                }
            }
            else
            {
                if ( !value.server_default )
                throw constraint_violation("'" + value.column_name(true) + "' is neither nullable nor has server_default.");
                else if ( value.null_has_been_set_explicitly() )
                throw constraint_violation("NULL value for column '"+ value.column_name(true) 
                    +"' violates not-null constraint. Hint : Try using server_default for this column.");
            }
            }
            else 
            {
            std::get<0>(output).push_back(typename Column::tag_type().column_name);
            std::get<1>(output).push_back(decorate(value.value()));
            }
    
        }
    };
    
    template<typename T>
    using sql_insert_formatter = formatter<T, sql_insert_adaptor>;
    
    class map_adaptor
    {
        public:
        using value_type = std::map<std::string, std::string>;
    
        template<std::size_t N, typename Value>
        void write(bool has_value, char const (&name)[N], Value && value, value_type & output)
        {
            if ( has_value )
            {
            output[name] = decorate(std::forward<Value>(value));
            }
        }
    };
    
    template<typename Object, typename Adaptor>
    auto object_format(Object && object, Adaptor && adaptor) -> typename std::remove_reference<Adaptor>::type::value_type
    {
        using object_type = typename std::remove_reference<Object>::type;
        using adaptor_type = typename std::remove_reference<Adaptor>::type;
    
        return formatter<object_type, adaptor_type>(std::forward<Object>(object)).output();
    }
    

}}} //tagsql # developement # formatting
