#pragma once

#ifndef DISTANT_CONFIG
#	define DISTANT_CONFIG
#	if defined (BOOST_USE_WINDOWS_H)
#		define WIN32_LEAN_AND_MEAN
#		include <Windows.h>
#		include <boost\winapi\basic_types.hpp>
#		include <TlHelp32.h>
#		include <Psapi.h>
#	endif
#
#	ifdef __cpp_lib_filesystem 
#		include <filesystem>
#	elif defined(__cpp_lib_experimental_filesystem)
#		include <experimental\filesystem>
#	else
#		include <boost/filesystem.hpp>
#	endif
#endif
