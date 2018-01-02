#pragma once

#include <boost\winapi\basic_types.hpp>
#include <boost\config\platform\win32.hpp>
#include <distant\support\winapi\basic_types.hpp>

#if !defined (BOOST_USE_WINDOWS_H)

struct PRIVILEGE_SET;

BOOST_SYMBOL_IMPORT boost::winapi::BOOL_ WINAPI PrivilegeCheck(
	boost::winapi::HANDLE_ ClientToken,
	::PRIVILEGE_SET* RequiredPrivileges,
	boost::winapi::LPBOOL_ pfResult);

#if !defined(BOOST_NO_ANSI_APIS)
BOOST_SYMBOL_IMPORT boost::winapi::BOOL_ WINAPI LookupPrivilegeDisplayNameA(
	boost::winapi::LPCSTR_ lpSystemName,
	boost::winapi::LPCSTR_ lpName,
	_Out_writes_to_opt_(*cchDisplayName, *cchDisplayName + 1) boost::winapi::LPSTR_ lpDisplayName,
	boost::winapi::LPDWORD_ cchDisplayName,
	boost::winapi::LPDWORD_ lpLanguageId);
#endif

BOOST_SYMBOL_IMPORT boost::winapi::BOOL_ WINAPI LookupPrivilegeDisplayNameW(
	boost::winapi::LPCWSTR_ lpSystemName,
	boost::winapi::LPCWSTR_ lpName,
	_Out_writes_to_opt_(*cchDisplayName, *cchDisplayName + 1) boost::winapi::LPWSTR_ lpDisplayName,
	boost::winapi::LPDWORD_ cchDisplayName,
	boost::winapi::LPDWORD_ lpLanguageId);


#if !defined(BOOST_NO_ANSI_APIS)
BOOST_SYMBOL_IMPORT boost::winapi::BOOL_ WINAPI LookupPrivilegeNameA(
	boost::winapi::LPCSTR_ lpSystemName,
	boost::winapi::PLUID_ lpLuid,
	_Out_writes_to_opt_(*cchDisplayName, *cchDisplayName + 1) boost::winapi::LPSTR_ lpName,
	boost::winapi::LPDWORD_ cchName);
#endif

BOOST_SYMBOL_IMPORT boost::winapi::BOOL_ WINAPI LookupPrivilegeNameW(
	boost::winapi::LPCWSTR_ lpSystemName,
	boost::winapi::PLUID_ lpLuid,
	_Out_writes_to_opt_(*cchDisplayName, *cchDisplayName + 1) boost::winapi::LPWSTR_ lpName,
	boost::winapi::LPDWORD_ cchName);

#if !defined(BOOST_NO_ANSI_APIS)
BOOST_SYMBOL_IMPORT boost::winapi::BOOL_ WINAPI LookupPrivilegeValueA(
	boost::winapi::LPCSTR_ lpSystemName,
	boost::winapi::LPCSTR_ lpName,
	boost::winapi::PLUID_ lpLuid);
#endif

BOOST_SYMBOL_IMPORT boost::winapi::BOOL_ WINAPI LookupPrivilegeValueW(
	boost::winapi::LPCWSTR_ lpSystemName,
	boost::winapi::LPCWSTR_ lpName,
	boost::winapi::PLUID_ lpLuid);

#endif // !defined BOOST_USE_WINDOWS_H

namespace boost::winapi {

#if !defined (BOOST_USE_WINDOWS_H)

	//
	//  Privilege Set - This is defined for a privilege set of one.
	//                  If more than one privilege is needed, then this structure
	//                  will need to be allocated with more space.
	//
	//  Note: don't change this structure without fixing the INITIAL_PRIVILEGE_SET
	//  structure (defined in se.h)
	////

	typedef struct _PRIVILEGE_SET_ {
		DWORD_ PrivilegeCount;
		DWORD_ Control;
		LUID_AND_ATTRIBUTES_ Privilege[ANYSIZE_ARRAY_];
	} PRIVILEGE_SET_, *PPRIVILEGE_SET_;

	constexpr boost::winapi::DWORD_ SE_PRIVILEGE_ENABLED_BY_DEFAULT_ = 0x00000001L;
	constexpr boost::winapi::DWORD_ SE_PRIVILEGE_ENABLED_ = 0x00000002L;
	constexpr boost::winapi::DWORD_ SE_PRIVILEGE_REMOVED_ = 0X00000004L;
	constexpr boost::winapi::DWORD_ SE_PRIVILEGE_USED_FOR_ACCESS_ = 0x80000000L;

	constexpr boost::winapi::DWORD_ SE_PRIVILEGE_VALID_ATTRIBUTES_ =
		SE_PRIVILEGE_ENABLED_BY_DEFAULT_ | SE_PRIVILEGE_ENABLED_ |
		SE_PRIVILEGE_REMOVED_ | SE_PRIVILEGE_USED_FOR_ACCESS_;

	// Privilege Set Control flags

	constexpr boost::winapi::DWORD_ PRIVILEGE_SET_ALL_NECESSARY_ = 1;

#else
	using PRIVILEGE_SET_ = ::PRIVILEGE_SET;
	using PPRIVILEGE_SET_ = ::PPRIVILEGE_SET;

	constexpr DWORD_ SE_PRIVILEGE_ENABLED_BY_DEFAULT_	= SE_PRIVILEGE_ENABLED_BY_DEFAULT;
	constexpr DWORD_ SE_PRIVILEGE_ENABLED_				= SE_PRIVILEGE_ENABLED;

	constexpr DWORD_ SE_PRIVILEGE_REMOVED_				= SE_PRIVILEGE_REMOVED;
	constexpr  DWORD_ SE_PRIVILEGE_USED_FOR_ACCESS_		= SE_PRIVILEGE_USED_FOR_ACCESS;
	constexpr DWORD_ SE_PRIVILEGE_VALID_ATTRIBUTES_		= SE_PRIVILEGE_VALID_ATTRIBUTES;
	
	// Privilege Set Control flags

	constexpr DWORD_ PRIVILEGE_SET_ALL_NECESSARY_ = PRIVILEGE_SET_ALL_NECESSARY;

#endif // !defined BOOST_USE_WINDOWS_H

	BOOST_FORCEINLINE boost::winapi::BOOL_ privilege_check(
		boost::winapi::HANDLE_ ClientToken,
		boost::winapi::PRIVILEGE_SET_* RequiredPrivileges,
		boost::winapi::LPBOOL_ pfResult)
	{
		return ::PrivilegeCheck(ClientToken, reinterpret_cast<PRIVILEGE_SET*>(RequiredPrivileges), pfResult);
	}

#if BOOST_USE_WINAPI_VERSION >= BOOST_WINAPI_VERSION_WINXP

#if !defined(BOOST_NO_ANSI_APIS)
	using ::LookupPrivilegeDisplayNameA;
#endif

	using ::LookupPrivilegeDisplayNameW;

#if !defined(BOOST_NO_ANSI_APIS)
	BOOST_FORCEINLINE boost::winapi::DWORD_ lookup_privilege_name(
		boost::winapi::LPCSTR_ lpSystemName,
		boost::winapi::PLUID_ lpLuid,
		_Out_writes_to_opt_(*cchDisplayName, *cchDisplayName + 1) boost::winapi::LPSTR_ lpName,
		boost::winapi::LPDWORD_ cchName)
	{
		return ::LookupPrivilegeNameA(
			lpSystemName,
			reinterpret_cast<LUID_*>(lpLuid),
			lpName,
			cchName);
	}
#endif

	BOOST_FORCEINLINE boost::winapi::DWORD_ lookup_privilege_name(
		boost::winapi::LPCWSTR_ lpSystemName,
		boost::winapi::PLUID_ lpLuid,
		_Out_writes_to_opt_(*cchDisplayName, *cchDisplayName + 1) boost::winapi::LPWSTR_ lpName,
		boost::winapi::LPDWORD_ cchName)
	{
		return ::LookupPrivilegeNameW(
			lpSystemName,
			reinterpret_cast<LUID_*>(lpLuid),
			lpName,
			cchName);
	}

#if !defined(BOOST_NO_ANSI_APIS)
	BOOST_FORCEINLINE boost::winapi::DWORD_ lookup_privilege_value(
		boost::winapi::LPCSTR_ lpSystemName,
		boost::winapi::LPCSTR_ lpName,
		boost::winapi::PLUID_ lpLuid)
	{
		return ::LookupPrivilegeValueA(
			lpSystemName,
			lpName, 
			reinterpret_cast<LUID_*>(lpLuid));
	}
#endif
	BOOST_FORCEINLINE boost::winapi::DWORD_ lookup_privilege_value(
		boost::winapi::LPCWSTR_ lpSystemName,
		boost::winapi::LPCWSTR_ lpName,
		boost::winapi::PLUID_ lpLuid)
	{
		return ::LookupPrivilegeValueW(
			lpSystemName,
			lpName,
			reinterpret_cast<LUID_*>(lpLuid));
	}

#endif


} // end namespace boost::winapi
