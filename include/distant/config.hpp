// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#ifndef BOOST_USE_WINDOWS_H
#define BOOST_USE_WINDOWS_H
#endif

#ifndef DISTANT_CONFIG
#	define DISTANT_CONFIG

#	ifndef __cpp_lib_experimental_filesystem
#		define __cpp_lib_experimental_filesystem
#	endif

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
