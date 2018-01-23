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

	// Privilege names
	constexpr auto SE_CREATE_TOKEN_NAME_              = TEXT("SeCreateTokenPrivilege");
	constexpr auto SE_ASSIGNPRIMARYTOKEN_NAME_        = TEXT("SeAssignPrimaryTokenPrivilege");
	constexpr auto SE_LOCK_MEMORY_NAME_               = TEXT("SeLockMemoryPrivilege");
	constexpr auto SE_INCREASE_QUOTA_NAME_            = TEXT("SeIncreaseQuotaPrivilege");
	constexpr auto SE_UNSOLICITED_INPUT_NAME_         = TEXT("SeUnsolicitedInputPrivilege");
	constexpr auto SE_MACHINE_ACCOUNT_NAME_           = TEXT("SeMachineAccountPrivilege");
	constexpr auto SE_TCB_NAME_                       = TEXT("SeTcbPrivilege");
	constexpr auto SE_SECURITY_NAME_                  = TEXT("SeSecurityPrivilege");
	constexpr auto SE_TAKE_OWNERSHIP_NAME_            = TEXT("SeTakeOwnershipPrivilege");
	constexpr auto SE_LOAD_DRIVER_NAME_               = TEXT("SeLoadDriverPrivilege");
	constexpr auto SE_SYSTEM_PROFILE_NAME_            = TEXT("SeSystemProfilePrivilege");
	constexpr auto SE_SYSTEMTIME_NAME_                = TEXT("SeSystemtimePrivilege");
	constexpr auto SE_PROF_SINGLE_PROCESS_NAME_       = TEXT("SeProfileSingleProcessPrivilege");
	constexpr auto SE_INC_BASE_PRIORITY_NAME_         = TEXT("SeIncreaseBasePriorityPrivilege");
	constexpr auto SE_CREATE_PAGEFILE_NAME_           = TEXT("SeCreatePagefilePrivilege");
	constexpr auto SE_CREATE_PERMANENT_NAME_          = TEXT("SeCreatePermanentPrivilege");
	constexpr auto SE_BACKUP_NAME_                    = TEXT("SeBackupPrivilege");
	constexpr auto SE_RESTORE_NAME_                   = TEXT("SeRestorePrivilege");
	constexpr auto SE_SHUTDOWN_NAME_                  = TEXT("SeShutdownPrivilege");
	constexpr auto SE_DEBUG_NAME_                     = TEXT("SeDebugPrivilege");
	constexpr auto SE_AUDIT_NAME_                     = TEXT("SeAuditPrivilege");
	constexpr auto SE_SYSTEM_ENVIRONMENT_NAME_        = TEXT("SeSystemEnvironmentPrivilege");
	constexpr auto SE_CHANGE_NOTIFY_NAME_             = TEXT("SeChangeNotifyPrivilege");
	constexpr auto SE_REMOTE_SHUTDOWN_NAME_           = TEXT("SeRemoteShutdownPrivilege");
	constexpr auto SE_UNDOCK_NAME_                    = TEXT("SeUndockPrivilege");
	constexpr auto SE_SYNC_AGENT_NAME_                = TEXT("SeSyncAgentPrivilege");
	constexpr auto SE_ENABLE_DELEGATION_NAME_         = TEXT("SeEnableDelegationPrivilege");
	constexpr auto SE_MANAGE_VOLUME_NAME_             = TEXT("SeManageVolumePrivilege");
	constexpr auto SE_IMPERSONATE_NAME_               = TEXT("SeImpersonatePrivilege");
	constexpr auto SE_CREATE_GLOBAL_NAME_             = TEXT("SeCreateGlobalPrivilege");
	constexpr auto SE_TRUSTED_CREDMAN_ACCESS_NAME_    = TEXT("SeTrustedCredManAccessPrivilege");
	constexpr auto SE_RELABEL_NAME_                   = TEXT("SeRelabelPrivilege");
	constexpr auto SE_INC_WORKING_SET_NAME_           = TEXT("SeIncreaseWorkingSetPrivilege");
	constexpr auto SE_TIME_ZONE_NAME_                 = TEXT("SeTimeZonePrivilege");
	constexpr auto SE_CREATE_SYMBOLIC_LINK_NAME_      = TEXT("SeCreateSymbolicLinkPrivilege");

#else
	// Privilege names
	constexpr auto SE_CREATE_TOKEN_NAME_              = SE_CREATE_TOKEN_NAME;
	constexpr auto SE_ASSIGNPRIMARYTOKEN_NAME_        = SE_ASSIGNPRIMARYTOKEN_NAME;
	constexpr auto SE_LOCK_MEMORY_NAME_               = SE_LOCK_MEMORY_NAME;
	constexpr auto SE_INCREASE_QUOTA_NAME_            = SE_INCREASE_QUOTA_NAME;
	constexpr auto SE_UNSOLICITED_INPUT_NAME_         = SE_UNSOLICITED_INPUT_NAME;
	constexpr auto SE_MACHINE_ACCOUNT_NAME_           = SE_MACHINE_ACCOUNT_NAME;
	constexpr auto SE_TCB_NAME_                       = SE_TCB_NAME;
	constexpr auto SE_SECURITY_NAME_                  = SE_SECURITY_NAME;
	constexpr auto SE_TAKE_OWNERSHIP_NAME_            = SE_TAKE_OWNERSHIP_NAME;
	constexpr auto SE_LOAD_DRIVER_NAME_               = SE_LOAD_DRIVER_NAME;
	constexpr auto SE_SYSTEM_PROFILE_NAME_            = SE_SYSTEM_PROFILE_NAME;
	constexpr auto SE_SYSTEMTIME_NAME_                = SE_SYSTEMTIME_NAME;
	constexpr auto SE_PROF_SINGLE_PROCESS_NAME_       = SE_PROF_SINGLE_PROCESS_NAME;
	constexpr auto SE_INC_BASE_PRIORITY_NAME_         = SE_INC_BASE_PRIORITY_NAME;
	constexpr auto SE_CREATE_PAGEFILE_NAME_           = SE_CREATE_PAGEFILE_NAME;
	constexpr auto SE_CREATE_PERMANENT_NAME_          = SE_CREATE_PERMANENT_NAME;
	constexpr auto SE_BACKUP_NAME_                    = SE_BACKUP_NAME;
	constexpr auto SE_RESTORE_NAME_                   = SE_RESTORE_NAME;
	constexpr auto SE_SHUTDOWN_NAME_                  = SE_SHUTDOWN_NAME;
	constexpr auto SE_DEBUG_NAME_                     = SE_DEBUG_NAME;
	constexpr auto SE_AUDIT_NAME_                     = SE_AUDIT_NAME;
	constexpr auto SE_SYSTEM_ENVIRONMENT_NAME_        = SE_SYSTEM_ENVIRONMENT_NAME;
	constexpr auto SE_CHANGE_NOTIFY_NAME_             = SE_CHANGE_NOTIFY_NAME;
	constexpr auto SE_REMOTE_SHUTDOWN_NAME_           = SE_REMOTE_SHUTDOWN_NAME;
	constexpr auto SE_UNDOCK_NAME_                    = SE_UNDOCK_NAME;
	constexpr auto SE_SYNC_AGENT_NAME_                = SE_SYNC_AGENT_NAME;
	constexpr auto SE_ENABLE_DELEGATION_NAME_         = SE_ENABLE_DELEGATION_NAME;
	constexpr auto SE_MANAGE_VOLUME_NAME_             = SE_MANAGE_VOLUME_NAME;
	constexpr auto SE_IMPERSONATE_NAME_               = SE_IMPERSONATE_NAME;
	constexpr auto SE_CREATE_GLOBAL_NAME_             = SE_CREATE_GLOBAL_NAME;
	constexpr auto SE_TRUSTED_CREDMAN_ACCESS_NAME_    = SE_TRUSTED_CREDMAN_ACCESS_NAME;
	constexpr auto SE_RELABEL_NAME_                   = SE_RELABEL_NAME;
	constexpr auto SE_INC_WORKING_SET_NAME_           = SE_INC_WORKING_SET_NAME;
	constexpr auto SE_TIME_ZONE_NAME_                 = SE_TIME_ZONE_NAME;
	constexpr auto SE_CREATE_SYMBOLIC_LINK_NAME_      = SE_CREATE_SYMBOLIC_LINK_NAME;

	using PRIVILEGE_SET_ = ::PRIVILEGE_SET;
	using PPRIVILEGE_SET_ = ::PPRIVILEGE_SET;

	constexpr DWORD_ SE_PRIVILEGE_ENABLED_BY_DEFAULT_	= SE_PRIVILEGE_ENABLED_BY_DEFAULT;
	constexpr DWORD_ SE_PRIVILEGE_ENABLED_				= SE_PRIVILEGE_ENABLED;

	constexpr DWORD_ SE_PRIVILEGE_REMOVED_				= SE_PRIVILEGE_REMOVED;
	constexpr DWORD_ SE_PRIVILEGE_USED_FOR_ACCESS_		= SE_PRIVILEGE_USED_FOR_ACCESS;
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
