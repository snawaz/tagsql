





#pragma once


#include <string>


namespace tagsql { namespace developement
{
    
    struct condition
    {
        std::string _value;
    
        condition(std::string value) : _value(std::move(value)) {}
    
        condition operator && (condition cond) const
        {
        return {_value + " AND " + cond._value };
        }
    };
    

}} //tagsql # developement
