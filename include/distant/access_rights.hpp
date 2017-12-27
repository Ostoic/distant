#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

//#include <windows.h>

#include <distant\utility\enum_operators.hpp>

#include <boost\winapi\access_rights.hpp>
#include <boost\winapi\handle_info.hpp>

#include <distant\support\winapi\process.hpp>
#include <boost\winapi\process.hpp>

namespace distant {

/// Contains enums for the following access rights:
///	process, token, handle, file, standard
struct access_rights
{
	enum class standard
	{
		Delete = boost::winapi::DELETE_,
		read_control = boost::winapi::READ_CONTROL_,
		synchronize = boost::winapi::SYNCHRONIZE_,
		write_dac = boost::winapi::WRITE_DAC_,
		writer_owner = boost::winapi::WRITE_OWNER_
	};

	enum class process
	{
		all_access = boost::winapi::PROCESS_ALL_ACCESS_,

		set_information = boost::winapi::PROCESS_SET_INFORMATION_,
		set_quota = boost::winapi::PROCESS_SET_QUOTA_,

		vm_operation = boost::winapi::PROCESS_VM_OPERATION_,
		vm_read = boost::winapi::PROCESS_VM_READ_,
		vm_write = boost::winapi::PROCESS_VM_WRITE_,

		create_process = boost::winapi::PROCESS_CREATE_PROCESS_,
		create_thread = boost::winapi::PROCESS_CREATE_THREAD_,
		dup_handle = boost::winapi::PROCESS_DUP_HANDLE_,

		suspend_resume = boost::winapi::PROCESS_SUSPEND_RESUME_,
		terminate = boost::winapi::PROCESS_TERMINATE_,

		// boost::winapi doesn't have query_limited_information?
		query_limited_information = distant::winapi::PROCESS_QUERY_LIMITED_INFORMATION_,
		query_information = boost::winapi::PROCESS_QUERY_INFORMATION_,

		synchronize = boost::winapi::SYNCHRONIZE_,
	};

	enum class token
	{
		assign_primary = 0x0001,		// Required to attach a primary token to a process. The SE_ASSIGNPRIMARYTOKEN_NAME privilege is also required to accomplish this task.
		duplicate = 0x0002,				// Required to duplicate an access token.
		impersonate = 0x0004,			// Required to attach an impersonation access token to a process.
		query = 0x0008,					// Required to query an access token.
		query_source = 0x0010,			// Required to query the source of an access token.
		adjust_privileges = 0x0020,		// Required to enable or disable the privileges in an access token.
		adjust_groups = 0x0040,			// Required to adjust the attributes of the groups in an access token.
		adjust_default = 0x0080, 		// Required to change the default owner, primary group, or DACL of an access token.
		adjust_sessionId = 0x0100,		// Required to adjust the session ID of an access token. The SE_TCB_NAME privilege is required.

		// XX ???
		trust_constraint_mask = boost::winapi::STANDARD_RIGHTS_READ_ | query | query_source,

		// Combines STANDARD_RIGHTS_READ and TOKEN_QUERY.
		read = boost::winapi::STANDARD_RIGHTS_READ_ | query,

		// Combines STANDARD_RIGHTS_WRITE, TOKEN_ADJUST_PRIVILEGES, TOKEN_ADJUST_GROUPS, and TOKEN_ADJUST_DEFAULT.
		write = boost::winapi::STANDARD_RIGHTS_WRITE_ | adjust_privileges |
		adjust_groups | adjust_default,

		// Combines STANDARD_RIGHTS_EXECUTE and TOKEN_IMPERSONATE.
		execute = boost::winapi::STANDARD_RIGHTS_EXECUTE_, 

		// Combines all possible access rights for a token.
		all_access = boost::winapi::STANDARD_RIGHTS_REQUIRED_ |
			assign_primary | duplicate | impersonate | query | query_source |
			adjust_privileges | adjust_groups | adjust_default
#if BOOST_USE_WINAPI_VERSION >= BOOST_WINAPI_VERSION_WIN2K
			| adjust_sessionId,
#endif
		
	};

	enum class handle
	{
		inherit = boost::winapi::HANDLE_FLAG_INHERIT_,					  // Child process will inherit object handle
		close_protected = boost::winapi::HANDLE_FLAG_PROTECT_FROM_CLOSE_, // Prevent CloseHandle from closing handle
	};
};

// Define flag operators for use with conforming access_rights
DEFINE_CONSTEXPR_ENUM_FLAG_OPERATORS(access_rights::process);
DEFINE_CONSTEXPR_ENUM_FLAG_OPERATORS(access_rights::token);
DEFINE_CONSTEXPR_ENUM_FLAG_OPERATORS(access_rights::standard);

} // end namespace distant
