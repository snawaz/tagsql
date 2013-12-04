
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
	return 0;

	//tagsql::meta::metadata meta("test", "localhost", 5432, "snawaz", "itsnotme");
	//tagsql::meta::metadata meta("justdial", "96.126.102.135", 5432, "triloq", "itsnotme");
	//tagsql::metadata meta("GooglePlaces", "96.126.102.135", 5432, "triloq", "itsnotme");
	//		std::cout << std::setfill('*');
	for(auto const & table : meta.meta_tables())
	{
	    std::cout << table.name << std::endl;
	    for(auto const & column : table.columns)
	    {
		std::cout << std::setw(18) << column.name << std::setw(28) 
		    /*<< std::setfill('*') */ << ("(" + column.type + ")");
		std::cout << std::setw(12) << (column.is_nullable? "nullable" : "");
		std::cout << std::setw(15) << (column.has_server_default? "server_default" : "");
		if ( column.is_primary_key )
		    std::cout << std::setw(20) << "primary_key";
		else if ( column.is_foreign_key )
		    std::cout << std::setw(20) << "foreign_key {" << column.ref_table_name << "::" 
			<< column.ref_column_name << "}";
		else
		    std::cout << std::setw(20);
		std::cout << std::endl;
	    }
	}

    }
    catch (const libconfig::ParseException &e)
    {
	std::cerr << e.what() << ": " << e.getError() << " in '" << e.getFile() << "' at line " << e.getLine() <<"."<< std::endl;
    }
    catch (const libconfig::SettingException &e)
    {
	std::cerr << e.what() << std::endl;
	std::cerr << e.getPath() << std::endl;
    }
    catch (const libconfig::ConfigException &e)
    {
	std::cerr << "ConfigException" << std::endl;
    }
    catch (const tagsql::meta::mapped_type_not_found &e)
    {
	std::cerr << "mapped_type_not_found: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
	std::cerr << "std::exception: " << e.what() << std::endl;
    }
}
