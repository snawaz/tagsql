
#pragma once

#include <ostream>
#include <string>
#include <stdexcept>
#include <map>

#include <libconfig.h++>

namespace tagsql { namespace meta 
{
    struct mapped_type
    {
	std::string type;
	std::string header;
    };

    std::ostream & operator << (std::ostream & out, mapped_type const & m)
    {
	return out << "{type=" << m.type << ", header=" << (m.header.empty()? "None" : m.header) <<"}";
    }
    
    struct mapped_type_not_found : public std::runtime_error
    {
	mapped_type_not_found(std::string const & sql_type) : 
	    std::runtime_error("The sql type '" + sql_type + "' doesn't map to any C++ type. Please add it to the .cfg file under type_mapping setting.") {}
    };

    class type_mapper
    {
	public:
	    type_mapper(libconfig::Setting & setting) 
	    {
		for(int i = 0 ; i < setting.getLength(); ++i)
		{
		    std::string sql_type = setting[i].getName();
		    mapped_type m;
		    if ( !setting[i].lookupValue("type", m.type) )
			throw mapped_type_not_found(sql_type);
		    setting[i].lookupValue("header", m.header);
		    _type_map[sql_type] = m;
		}
	    }
	    mapped_type operator[](std::string const & sql_type)
	    {
		for(auto const & p : _type_map)
		{
		    auto const & key = p.first;
		    if ( key == sql_type )
			return p.second;
		    else if ( key.front() == '*' )
		    {
			if ( sql_type.find(&key[1], 1) != std::string::npos)
			    return p.second;
		    }
		    else if ( key.back() == '*' )
		    {
			if ( sql_type.find(&key[0], 0, key.size() - 1) != std::string::npos)
			    return p.second;
		    }
		}
		throw mapped_type_not_found(sql_type);
	    }
	private:
	    std::map<std::string,mapped_type>  _type_map;
    };

}} //tagsql # meta
