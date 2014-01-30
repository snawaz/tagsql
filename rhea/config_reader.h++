
#pragma once

#include <ostream>
#include <string>
#include <stdexcept>
#include <map>

#include <libconfig.h++>

#include <foam/strlib/format.h>
#include <foam/strlib/strlib.h>
#include <tagsql/rhea/type_mapper.h++>

namespace tagsql { namespace rhea 
{
	struct config_error : public std::runtime_error
	{
		using std::runtime_error::runtime_error;
	};

	struct connection_info
	{
		std::string dbname;
		std::string host;
		int         port;
		std::string user;
		std::string password;
	};

	std::ostream& operator<<(std::ostream & out, connection_info const & info)
	{
		namespace fs = ::foam::strlib;
		return fs::print(out, "{{{0},{1},{2},{3},{4}}}", info.dbname, info.host, info.port, info.user, info.password);
 	}

	class config_reader
	{
	public:
		config_reader(std::string config_file) : _config_file(std::move(config_file))
		{
			_config.readFile(_config_file.c_str());
		}
		::tagsql::rhea::type_mapper type_mapper()
		{
			return find("type_mapping");
		}
		std::string operator[](std::string const & key)
		{
			return find(key);
		}
		connection_info connection()
		{
			return { find("connection.dbname"), find("connection.host"), find("connection.port"), find("connection.user"), find("connection.password") };
		}
	private:
		libconfig::Setting& find(std::string const & key)
		{
			auto it = _settings.find(key);
			if ( it != _settings.end() )
				return *(it->second);
			else if ( !_config.exists(key) )
				throw config_error(key + " not found.");
			auto & value = _config.lookup(key);
			_settings[key] = &value;
			return value;
		}
	private:
		std::string                               _config_file;
		libconfig::Config                         _config;
		std::map<std::string,libconfig::Setting*>  _settings;
	};

}} //tagsql # meta
