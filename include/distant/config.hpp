#pragma once

#if defined (BOOST_USE_WINDOWS_H)
#define WIN32_LEAN_AND_MEAN
#include <boost\winapi\basic_types.hpp>
#include <TlHelp32.h>
#include <Psapi.h>
#endif
