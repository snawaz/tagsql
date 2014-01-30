

#pragma once

namespace posix
{
	#include <sys/stat.h>
	#include <dirent.h>
	#include <string.h>
}

#include <string>
#include <vector>
#include <stdexcept>

#include <tagsql/common/exceptions.h++>
#include <foam/strlib/format.h>
#include <foam/strlib/strlib.h>

namespace tagsql
{
	//recursively ensure the directory path
	void ensure_dir(std::string const & dir_path) 
	{
		auto i = dir_path.rfind('/');
		if ( i != std::string::npos && i > 0 )
			ensure_dir(dir_path.substr(0, i));
		auto ret= ::posix::mkdir(dir_path.c_str(), 0777);
		if ( !ret )
			return;
		if ( errno != EEXIST )
			throw os_error(::foam::strlib::format("couldn't create directory '{0}'. errno = {1}, errmsg = {2}", dir_path, errno, posix::strerror(errno)));
	}

	template<typename Predicate>
	std::vector<std::string> read_directory(std::string const & dir_path, Predicate filter)
	{
		auto dir = ::posix::opendir(dir_path.c_str());
		if ( dir == nullptr )
			throw std::runtime_error("Failed to open directory '"+ dir_path +"' to read");

		std::vector<std::string> files;
		while( auto entry = ::posix::readdir(dir) )
		{
			if ( filter(entry->d_name) )
				files.emplace_back(entry->d_name);
		}
		::posix::closedir(dir);
		return files;
	}

}
