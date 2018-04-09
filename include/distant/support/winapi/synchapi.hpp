// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once


#include <boost/winapi/basic_types.hpp>

#if !defined (BOOST_USE_WINDOWS_H)

BOOST_SYMBOL_IMPORT boost::winapi::HANDLE_ WINAPI CreateMutexA(
	boost::winapi::LPSECURITY_ATTRIBUTES_ lpMutexAttributes,
	boost::winapi::BOOL_ bInitialOwner,
	boost::winapi::LPCSTR_ lpName
);


BOOST_SYMBOL_IMPORT boost::winapi::HANDLE_ WINAPI CreateMutexW(
	boost::winapi::LPSECURITY_ATTRIBUTES_ lpMutexAttributes,
	boost::winapi::BOOL_ bInitialOwner,
	boost::winapi::LPCWSTR_ lpName
);

BOOST_SYMBOL_IMPORT boost::winapi::HANDLE_ WINAPI OpenMutexA(
	boost::winapi::DWORD_ dwDesiredAccess,
	boost::winapi::BOOL_ bInheritHandle,
	boost::winapi::LPCSTR_ lpName
);

BOOST_SYMBOL_IMPORT boost::winapi::HANDLE_ WINAPI OpenMutexW(
	boost::winapi::DWORD_ dwDesiredAccess,
	boost::winapi::BOOL_ bInheritHandle,
	boost::winapi::LPCWSTR_ lpName
);

BOOST_SYMBOL_IMPORT boost::winapi::BOOL_ WINAPI ReleaseMutex(
	boost::winapi::HANDLE_ hMutex
);

#endif // !defined BOOST_USE_WINDOWS_H

namespace boost::winapi 
{

	using ::CreateMutexA;
	using ::CreateMutexW;
	using ::OpenMutexA;
	using ::OpenMutexW;
	using ::ReleaseMutex;

#if !defined(BOOST_NO_ANSI_APIS)
	BOOST_FORCEINLINE boost::winapi::HANDLE_ create_mutex(
		boost::winapi::LPSECURITY_ATTRIBUTES_ lpMutexAttributes,
		boost::winapi::BOOL_ bInitialOwner,
		boost::winapi::LPCSTR_ lpName)
	{
		return CreateMutexA(reinterpret_cast<LPSECURITY_ATTRIBUTES>(lpMutexAttributes), bInitialOwner, lpName);
	}

	BOOST_FORCEINLINE boost::winapi::HANDLE_ open_mutex(
		boost::winapi::DWORD_ dwDesiredAccess,
		boost::winapi::BOOL_ bInheritHandle,
		boost::winapi::LPCSTR_ lpName)
	{
		return OpenMutexA(dwDesiredAccess, bInheritHandle, lpName);
	}
#endif

	BOOST_FORCEINLINE boost::winapi::HANDLE_ create_mutex(
		boost::winapi::LPSECURITY_ATTRIBUTES_ lpMutexAttributes,
		boost::winapi::BOOL_ bInitialOwner,
		boost::winapi::LPCWSTR_ lpName)
	{
		return CreateMutexW(reinterpret_cast<LPSECURITY_ATTRIBUTES>(lpMutexAttributes), bInitialOwner, lpName);
	}

	BOOST_FORCEINLINE boost::winapi::HANDLE_ open_mutex(
		boost::winapi::DWORD_ dwDesiredAccess,
		boost::winapi::BOOL_ bInheritHandle,
		boost::winapi::LPCWSTR_ lpName)
	{
		return OpenMutexW(dwDesiredAccess, bInheritHandle, lpName);
	}
}
