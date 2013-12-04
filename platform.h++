

#pragma once

namespace posix
{
	#include <sys/stat.h>
        #include <dirent.h>
}

#include <string>
#include <vector>
#include <stdexcept>

namespace tagsql
{
    bool ensure_dir(std::string const & dir_path)
    {
	return  ! ::posix::mkdir(dir_path.c_str(), 0777);
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
