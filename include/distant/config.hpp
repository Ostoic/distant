#pragma once

#include <string_view>

#if defined (BOOST_USE_WINDOWS_H)
#include <TlHelp32.h>
#include <Psapi.h>
#endif

namespace distant::config {
#ifdef UNICODE
	using string = std::wstring;
	using string_view = std::wstring_view;
#else
	using string = std::wstring;
	using string_view = std::wstring_view;
#endif

#ifdef __cpp_lib_experimental_filesystem

#endif

#ifdef __cpp_lib_filesystem

#endif

	using string_elem = std::decay_t<string::reference>;
}
