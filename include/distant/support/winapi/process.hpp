#pragma once

#include <boost\winapi\basic_types.hpp>

#if !defined (BOOST_USE_WINDOWS_H)

#if BOOST_USE_WINAPI_VERSION >= BOOST_WINAPI_VERSION_WIN6

#if !defined(BOOST_NO_ANSI_APIS)
BOOST_SYMBOL_IMPORT boost::winapi::BOOL_ WINAPI QueryFullProcessImageNameA(
	boost::winapi::HANDLE_ hProcess,
	boost::winapi::DWORD_ dwFlags,
	_Out_writes_to_(*lpdwSize, *lpdwSize) boost::winapi::LPSTR_ lpExeName,
	boost::winapi::PDWORD_ lpdwSize);
#endif // !defined BOOST_NO_ANSI_APIS

BOOST_SYMBOL_IMPORT boost::winapi::BOOL_ WINAPI QueryFullProcessImageNameW(
	boost::winapi::HANDLE_ hProcess,
	boost::winapi::DWORD_ dwFlags,
	_Out_writes_to_(*lpdwSize, *lpdwSize) boost::winapi::LPWSTR_ lpExeName,
	boost::winapi::PDWORD_ lpdwSize);

#endif // >= win6 version check

#if BOOST_USE_WINAPI_VERSION >= BOOST_WINAPI_VERSION_WINXP
BOOST_SYMBOL_IMPORT boost::winapi::DWORD_ WINAPI GetProcessId(
	boost::winapi::HANDLE_ Process);
#endif // >= winxp version check


#endif // !defined BOOST_USE_WINDOWS_H

namespace boost  {
namespace winapi {

#if !defined (BOOST_USE_WINDOWS_H)

	constexpr boost::winapi::DWORD_ PROCESS_QUERY_LIMITED_INFORMATION_ = 0x1000;
	constexpr boost::winapi::DWORD_ PROCESS_SET_LIMITED_INFORMATION_ = 0x2000;

#else
	constexpr DWORD_ PROCESS_QUERY_LIMITED_INFORMATION_ = PROCESS_QUERY_LIMITED_INFORMATION;
	constexpr DWORD_ PROCESS_SET_LIMITED_INFORMATION_ = PROCESS_SET_LIMITED_INFORMATION;

#endif // !defined BOOST_USE_WINDOWS_H

	using ::GetProcessId;

#if (BOOST_USE_WINAPI_VERSION >= BOOST_WINAPI_VERSION_WIN6)
#if !defined(BOOST_NO_ANSI_APIS)
	BOOST_FORCEINLINE boost::winapi::BOOL_ query_full_process_image_name(
		boost::winapi::HANDLE_ hProcess,
		boost::winapi::DWORD_ dwFlags,
		boost::winapi::LPSTR_ lpExeName,
		boost::winapi::PDWORD_ lpdwSize)
	{
		return ::QueryFullProcessImageNameA(
			hProcess,
			dwFlags,
			lpExeName,
			lpdwSize);
	}
#endif // end ansi api check

	BOOST_FORCEINLINE boost::winapi::BOOL_ query_full_process_image_name(
		boost::winapi::HANDLE_ hProcess,
		boost::winapi::DWORD_ dwFlags,
		boost::winapi::LPWSTR_ lpExeName,
		boost::winapi::PDWORD_ lpdwSize)
	{
		return ::QueryFullProcessImageNameW(
			hProcess,
			dwFlags,
			lpExeName,
			lpdwSize);
	}
#endif // end api version check

} // end namespace winapi
} // end namespace boost