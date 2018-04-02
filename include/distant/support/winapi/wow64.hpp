#pragma once

#include <boost/winapi/basic_types.hpp>

#if !defined (BOOST_USE_WINDOWS_H)
#if BOOST_USE_WINAPI_VERSION >= BOOST_WINAPI_VERSION_WINXP

BOOST_SYMBOL_IMPORT boost::winapi::BOOL_ WINAPI IsWow64Process(
	boost::winapi::HANDLE_ hProcess,
	boost::winapi::PBOOL_ Wow64Process);

#endif // winxp version check
#endif // if not using windows.h


namespace boost::winapi {

#if BOOST_USE_WINAPI_VERSION >= BOOST_WINAPI_VERSION_WINXP
	using ::IsWow64Process;
#endif

} // namespace boost::winapi
