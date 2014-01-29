
#include <iostream>
#include <iomanip>

#include "metadata.h++"
#include "config_reader.h++"
#include "type_mapper.h++"
#include "code_generator.h++"

template<typename Item>
void line(Item const & item) 
{
    std::cout << "+ " << item << "\n"; 
}

int main()
{	
	try
	{
		tagsql::meta::config_reader config("test.cfg");
		auto mapper = config.type_mapper();
		auto cinfo = config.connection();
		tagsql::meta::metadata meta(cinfo.dbname, cinfo.host, cinfo.port, cinfo.user, cinfo.password);
		tagsql::code_generator(meta, config, mapper).generate();

		std::cout << "\n";
		std::cout << std::string(20, '=') << " Summary " << std::string(20, '=') << "\n";
		line(cinfo);
		line(config["namespace"]);
		line(mapper["name"]);
		line(mapper["bigint"]);
		line(mapper["timestamp*"]);
		line(mapper["smallint"]);
	}
	catch (libconfig::ParseException const & e )
	{
		std::cerr << e.what() << ": " << e.getError() << " in '" << e.getFile() << "' at line " << e.getLine() <<"."<< std::endl;
	}
	catch (libconfig::SettingException const & e)
	{
		std::cerr << e.what() << std::endl;
		std::cerr << e.getPath() << std::endl;
	}
	catch (libconfig::ConfigException const & e)
	{
		std::cerr << "ConfigException" << std::endl;
	}
	catch (tagsql::meta::mapped_type_not_found const & e)
	{
		std::cerr << "mapped_type_not_found: " << e.what() << std::endl;
	}
	catch (std::exception const & e)
	{
		std::cerr << "std::exception: " << e.what() << std::endl;
	}
}
