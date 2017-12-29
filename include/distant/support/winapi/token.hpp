#pragma once

#include <boost\winapi\basic_types.hpp>
#include <boost\winapi\access_rights.hpp>

#include <distant\support\winapi\basic_types.hpp>

#ifndef DISTANT_WINDOWS_INCLUDED
#include <Windows.h>
#define DISTANT_WINDOWS_INCLUDED
#endif

BOOST_SYMBOL_IMPORT boost::winapi::BOOL_ WINAPI AdjustTokenPrivileges(
	boost::winapi::HANDLE_ TokenHandle,
	boost::winapi::BOOL_ DisableAllPrivileges,
	::PTOKEN_PRIVILEGES NewState,
	boost::winapi::DWORD_ BufferLength,
	_Out_writes_bytes_to_opt_(BufferLength, *ReturnLength) ::PTOKEN_PRIVILEGES PreviousState,
	boost::winapi::PDWORD_ ReturnLength
);

namespace boost::winapi
{
	//
	// Token Specific Access Rights.
	//

	const boost::winapi::DWORD_ TOKEN_ASSIGN_PRIMARY_ = 0x0001;
	const boost::winapi::DWORD_ TOKEN_DUPLICATE_ = 0x0002;
	const boost::winapi::DWORD_ TOKEN_IMPERSONATE_ = 0x0004;
	const boost::winapi::DWORD_ TOKEN_QUERY_ = 0x0008;
	const boost::winapi::DWORD_ TOKEN_QUERY_SOURCE_ = 0x0010;
	const boost::winapi::DWORD_ TOKEN_ADJUST_PRIVILEGES_ = 0x0020;
	const boost::winapi::DWORD_  TOKEN_ADJUST_GROUPS_ = 0x0040;
	const boost::winapi::DWORD_  TOKEN_ADJUST_DEFAULT_ = 0x0080;
	const boost::winapi::DWORD_  TOKEN_ADJUST_SESSIONID_ = 0x0100;

	const boost::winapi::DWORD_ TOKEN_ALL_ACCESS_P_ = STANDARD_RIGHTS_REQUIRED  |
		TOKEN_ASSIGN_PRIMARY_	| TOKEN_DUPLICATE_			|
		TOKEN_IMPERSONATE_		| TOKEN_QUERY_				|
		TOKEN_QUERY_SOURCE_		| TOKEN_ADJUST_PRIVILEGES_	|
		TOKEN_ADJUST_GROUPS_	| TOKEN_ADJUST_DEFAULT_;

#if ((defined(_WIN32_WINNT) && (_WIN32_WINNT > 0x0400)) || (!defined(_WIN32_WINNT)))
	const boost::winapi::DWORD_ TOKEN_ALL_ACCESS_ = TOKEN_ALL_ACCESS_P_ | TOKEN_ADJUST_SESSIONID_;
#else
	const boost::winapi::DWORD_ TOKEN_ALL_ACCESS_ = TOKEN_ALL_ACCESS_P_;
#endif

	const boost::winapi::DWORD_ TOKEN_READ_ = boost::winapi::STANDARD_RIGHTS_READ_ | TOKEN_QUERY_;

	const boost::winapi::DWORD_ TOKEN_WRITE_ = boost::winapi::STANDARD_RIGHTS_WRITE_ | TOKEN_ADJUST_PRIVILEGES_ | TOKEN_ADJUST_GROUPS_ | TOKEN_ADJUST_DEFAULT_;
	const boost::winapi::DWORD_ TOKEN_EXECUTE_ = boost::winapi::STANDARD_RIGHTS_EXECUTE_;

	const boost::winapi::DWORD_ TOKEN_TRUST_CONSTRAINT_MASK_ = boost::winapi::STANDARD_RIGHTS_READ_ | TOKEN_QUERY_ | TOKEN_QUERY_SOURCE_;

	//
	// Token information class structures
	//

	typedef struct _TOKEN_PRIVILEGES_ {
		boost::winapi::DWORD_ PrivilegeCount;
		boost::winapi::LUID_AND_ATTRIBUTES_ Privileges[ANYSIZE_ARRAY];
	} TOKEN_PRIVILEGES_, *PTOKEN_PRIVILEGES_;

	typedef struct _PRIVILEGE_SET_ {
		boost::winapi::DWORD_ PrivilegeCount;
		boost::winapi::DWORD_ Control;
		boost::winapi::LUID_AND_ATTRIBUTES_ Privilege[ANYSIZE_ARRAY];
	} PRIVILEGE_SET_, *PPRIVILEGE_SET_;

	BOOST_FORCEINLINE boost::winapi::DWORD_ adjust_token_privilege(
		boost::winapi::HANDLE_ TokenHandle,
		boost::winapi::BOOL_ DisableAllPrivileges,
		boost::winapi::PTOKEN_PRIVILEGES_ NewState,
		boost::winapi::DWORD_ BufferLength,
		_Out_writes_bytes_to_opt_(BufferLength, *ReturnLength) boost::winapi::PTOKEN_PRIVILEGES_ PreviousState,
		boost::winapi::PDWORD_ ReturnLength)
	{
		return ::AdjustTokenPrivileges(
			TokenHandle,
			DisableAllPrivileges,
			reinterpret_cast<TOKEN_PRIVILEGES*>(NewState),
			BufferLength,
			reinterpret_cast<TOKEN_PRIVILEGES*>(PreviousState),
			ReturnLength);
	}
}
