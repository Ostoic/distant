#pragma once

#include <boost/winapi/access_rights.hpp>

#include <distant/support/winapi/basic_types.hpp>

#if !defined (BOOST_USE_WINDOWS_H)

struct TOKEN_PRIVILEGES;

BOOST_SYMBOL_IMPORT boost::winapi::BOOL_ WINAPI AdjustTokenPrivileges(
	boost::winapi::HANDLE_ TokenHandle,
	boost::winapi::BOOL_ DisableAllPrivileges,
	::TOKEN_PRIVILEGES* NewState,
	boost::winapi::DWORD_ BufferLength,
	_Out_writes_bytes_to_opt_(BufferLength, *ReturnLength) ::TOKEN_PRIVILEGES* PreviousState,
	boost::winapi::PDWORD_ ReturnLength);

BOOST_SYMBOL_IMPORT boost::winapi::BOOL_ WINAPI SetThreadToken(
	boost::winapi::PHANDLE_ Thread,
	boost::winapi::HANDLE_ Token);

BOOST_SYMBOL_IMPORT boost::winapi::BOOL_ WINAPI OpenProcessToken(
	boost::winapi::HANDLE_ ProcessHandle,
	boost::winapi::DWORD_ DesiredAccess,
	boost::winapi::PHANDLE_ TokenHandle);

BOOST_SYMBOL_IMPORT boost::winapi::BOOL_ WINAPI OpenThreadToken(
	boost::winapi::HANDLE_ ThreadHandle,
	boost::winapi::DWORD_ DesiredAccess,
	boost::winapi::BOOL_ OpenAsSelf,
	boost::winapi::PHANDLE_ TokenHandle);

#endif // !defined BOOST_USE_WINDOWS_H

namespace boost::winapi
{
	//
	// Token Specific Access Rights.
	//

#if !defined (BOOST_USE_WINDOWS_H)
	const boost::winapi::DWORD_ TOKEN_ASSIGN_PRIMARY_ = 0x0001;
	const boost::winapi::DWORD_ TOKEN_DUPLICATE_ = 0x0002;
	const boost::winapi::DWORD_ TOKEN_IMPERSONATE_ = 0x0004;
	const boost::winapi::DWORD_ TOKEN_QUERY_ = 0x0008;
	const boost::winapi::DWORD_ TOKEN_QUERY_SOURCE_ = 0x0010;
	const boost::winapi::DWORD_ TOKEN_ADJUST_PRIVILEGES_ = 0x0020;
	const boost::winapi::DWORD_  TOKEN_ADJUST_GROUPS_ = 0x0040;
	const boost::winapi::DWORD_  TOKEN_ADJUST_DEFAULT_ = 0x0080;
	const boost::winapi::DWORD_  TOKEN_ADJUST_SESSIONID_ = 0x0100;

	const boost::winapi::DWORD_ TOKEN_ALL_ACCESS_P_ = STANDARD_RIGHTS_REQUIRED_  |
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

#else
	const DWORD_ TOKEN_ASSIGN_PRIMARY_ = TOKEN_ASSIGN_PRIMARY;
	const DWORD_  TOKEN_DUPLICATE_		= TOKEN_DUPLICATE;
	const DWORD_  TOKEN_IMPERSONATE_	= TOKEN_IMPERSONATE;
	const DWORD_  TOKEN_QUERY_			= TOKEN_QUERY;
	const DWORD_  TOKEN_QUERY_SOURCE_	= TOKEN_QUERY_SOURCE;
	const DWORD_  TOKEN_ADJUST_PRIVILEGES_ = TOKEN_ADJUST_PRIVILEGES;
	const DWORD_  TOKEN_ADJUST_GROUPS_	= TOKEN_ADJUST_GROUPS;
	const DWORD_  TOKEN_ADJUST_DEFAULT_ = TOKEN_ADJUST_DEFAULT;
	const DWORD_  TOKEN_ADJUST_SESSIONID_ = TOKEN_ADJUST_SESSIONID;

	const DWORD_  TOKEN_ALL_ACCESS_P_	= TOKEN_ALL_ACCESS_P;
	const DWORD_  TOKEN_ALL_ACCESS_		= TOKEN_ALL_ACCESS;

	const DWORD_  TOKEN_READ_ = TOKEN_READ;
	const DWORD_  TOKEN_WRITE_ = TOKEN_WRITE;
	const DWORD_  TOKEN_EXECUTE_ = TOKEN_EXECUTE;
	const DWORD_  TOKEN_TRUST_CONSTRAINT_MASK_ = TOKEN_TRUST_CONSTRAINT_MASK;

#endif // !defined BOOST_USE_WINDOWS_H
	//
	// Token information class structures
	//

	using ::OpenProcessToken;
	using ::OpenThreadToken;
	using ::SetThreadToken;

#if !defined (BOOST_USE_WINDOWS_H)

	typedef struct _TOKEN_PRIVILEGES_ {
		boost::winapi::DWORD_ PrivilegeCount;
		boost::winapi::LUID_AND_ATTRIBUTES_ Privileges[ANYSIZE_ARRAY_];
	} TOKEN_PRIVILEGES_, *PTOKEN_PRIVILEGES_;

#else
	using _TOKEN_PRIVILEGES_ = ::_TOKEN_PRIVILEGES;
	using TOKEN_PRIVILEGES_ = ::TOKEN_PRIVILEGES;
	using PTOKEN_PRIVILEGES_ = ::PTOKEN_PRIVILEGES;

#endif // !defined BOOST_USE_WINDOWS_H


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
