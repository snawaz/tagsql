
#include <iostream>
#include <iomanip>

#include <tagsql/rhea/metadata.h++>
#include <tagsql/rhea/config_reader.h++>
#include <tagsql/rhea/type_mapper.h++>
#include <tagsql/rhea/code_generator.h++>

template<typename Item>
void line(Item const & item) 
{
    std::cout << "+ " << item << "\n"; 
}

void usage()
{
	namespace fs = ::foam::strlib;
	fs::println("Usage:");
	fs::println("\t./rhea <config.cfg>");
}

int main(int argc, char const *argv[])
{	
	try
	{
		if ( argc < 2 )
		{
			usage();
			return 0;
		}

		tagsql::rhea::config_reader config(argv[1]);
		auto mapper = config.type_mapper();
		auto cinfo = config.connection();
		tagsql::rhea::metadata meta(cinfo.dbname, cinfo.host, cinfo.port, cinfo.user, cinfo.password);
		tagsql::rhea::code_generator(meta, config, mapper).generate();

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
		std::cerr << "ParseException: " << e.what() << ": " << e.getError() << " in '" << e.getFile() << "' at line " << e.getLine() <<"."<< std::endl;
	}
	catch (libconfig::SettingException const & e)
	{
		std::cerr << e.what() << std::endl;
		std::cerr << e.getPath() << std::endl;
	}
	catch (libconfig::ConfigException const & e)
	{
		std::cerr << "ConfigException: " << e.what() << std::endl;
	}
	catch (tagsql::rhea::mapped_type_not_found const & e)
	{
		std::cerr << "mapped_type_not_found: " << e.what() << std::endl;
	}
	catch (std::exception const & e)
	{
		std::cerr << "std::exception: " << e.what() << std::endl;
	}
}
