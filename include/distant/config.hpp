#pragma once

#include <string_view>

#if defined (BOOST_USE_WINDOWS_H)
#define WIN32_LEAN_AND_MEAN
#include <boost\winapi\basic_types.hpp>
#include <TlHelp32.h>
#include <Psapi.h>
#endif

namespace distant::config {
#ifdef __cpp_lib_experimental_filesystem

#endif

#ifdef __cpp_lib_filesystem

#endif
}
