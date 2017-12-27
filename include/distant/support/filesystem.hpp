#pragma once

#ifdef __cpp_lib_filesystem 
#include <filesystem>
#elif defined(__cpp_lib_experimental_filesystem)
#include <experimental\filesystem>
#else
#include <boost/filesystem.hpp>
#endif

namespace distant
{
#ifdef __cpp_lib_filesystem
	namespace filesystem = std::filesystem;
#elif defined(__cpp_lib_experimental_filesystem)
	namespace filesystem = std::experimental::filesystem;
#else
	namespace filesystem = boost::filesystem;
#endif


}