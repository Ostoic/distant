#pragma once

#ifdef __cpp_lib_experimental_filesystem 
#include <experimental/filesystem>
#elseif defined(__cpp_lib_filesystem)
#include <filesystem>
#else
#include <filesystem>
#endif

namespace distant
{
#ifdef __cpp_lib_filesystem
	namespace filesystem = std::filesystem;
#else
	namespace filesystem = std::experimental::filesystem;
#endif


}