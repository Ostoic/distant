#pragma once

#ifndef BOOST_USE_WINDOWS_H
#define BOOST_USE_WINDOWS_H
#endif

#ifndef DISTANT_CONFIG
#	define DISTANT_CONFIG
#	define __cpp_lib_experimental_filesystem
#	include <distant/support/filesystem.hpp>
#
#	if defined (BOOST_USE_WINDOWS_H)
#		define WIN32_LEAN_AND_MEAN
#		include <Windows.h>
#		include <boost/winapi/basic_types.hpp>
#		include <TlHelp32.h>
#		include <Psapi.h>
#	endif
#endif
