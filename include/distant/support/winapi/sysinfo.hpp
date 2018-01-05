#pragma once

#include <boost\winapi\basic_types.hpp>

#if !defined (BOOST_USE_WINDOWS_H)

#if !defined (BOOST_NO_ANSI_APIS)
BOOST_SYMBOL_IMPORT boost::winapi::UINT_ WINAPI GetWindowsDirectoryA(
	boost::winapi::LPSTR_ lpBuffer,
	boost::winapi::UINT_ uSize);

#endif // !defined no ansi apis

BOOST_SYMBOL_IMPORT boost::winapi::UINT_ WINAPI GetWindowsDirectoryW(
	boost::winapi::LPWSTR_ lpBuffer,
	boost::winapi::UINT_ uSize);

#endif // !defined BOOST_USE_WINDOWS_H

namespace boost::winapi {

#if !defined (BOOST_NO_ANSI_APIS)
	using ::GetWindowsDirectoryA;
#endif

	using ::GetWindowsDirectoryW;
}
