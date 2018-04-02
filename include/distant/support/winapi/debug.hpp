#pragma once

#include <boost/winapi/basic_types.hpp>

#if !defined (BOOST_USE_WINDOWS_H)
BOOST_SYMBOL_IMPORT boost::winapi::BOOL_ WINAPI IsDebuggerPresent();

#if BOOST_USE_WINAPI_VERSION >= BOOST_WINAPI_VERSION_WINXP

BOOST_SYMBOL_IMPORT boost::winapi::BOOL_ WINAPI CheckRemoteDebuggerPresent(
	boost::winapi::HANDLE_ hProcess,
	boost::winapi::PBOOL_ pbDebuggerPresent);

#endif // winxp version check
#endif // not using windows.h

namespace boost::winapi {

	using ::IsDebuggerPresent;
	using ::CheckRemoteDebuggerPresent;

} // namespace boost::winapi
