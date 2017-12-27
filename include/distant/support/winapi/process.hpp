#pragma once

#include <boost\winapi\basic_types.hpp>

#if (BOOST_USE_WINAPI_VERSION >= BOOST_WINAPI_VERSION_WIN6)

#if !defined(BOOST_NO_ANSI_APIS)
BOOST_SYMBOL_IMPORT boost::winapi::BOOL_ WINAPI QueryFullProcessImageNameA(
	boost::winapi::HANDLE_ hProcess,
	boost::winapi::DWORD_ dwFlags,
	_Out_writes_to_(*lpdwSize, *lpdwSize) boost::winapi::LPSTR_ lpExeName,
	boost::winapi::PDWORD_ lpdwSize);
#endif // end ansi api check

BOOST_SYMBOL_IMPORT boost::winapi::BOOL_ WINAPI QueryFullProcessImageNameW(
	boost::winapi::HANDLE_ hProcess,
	boost::winapi::DWORD_ dwFlags,
	_Out_writes_to_(*lpdwSize, *lpdwSize) boost::winapi::LPWSTR_ lpExeName,
	boost::winapi::PDWORD_ lpdwSize);

#endif // end version check

namespace distant {
namespace winapi  {

	const boost::winapi::DWORD_ PROCESS_QUERY_LIMITED_INFORMATION_ = 0x1000;
	const boost::winapi::DWORD_ PROCESS_SET_LIMITED_INFORMATION_ = 0x2000;  

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
#endif // end version check

} // end namespace winapi 
} // end namespace boost