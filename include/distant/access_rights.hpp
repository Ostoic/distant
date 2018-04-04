#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <distant/utility/enum_operators.hpp>

#include <boost/winapi/access_rights.hpp>
#include <boost/winapi/handle_info.hpp>

#include <distant/support/winapi/process.hpp>
#include <distant/support/winapi/token.hpp>
#include <boost/winapi/process.hpp>
#include <boost/winapi/page_protection_flags.hpp>

namespace distant
{
	/// Contains enums for the following access rights:
	///	process, token, handle, file, standard
	struct access_rights
	{
		enum class standard
		{
			delete_ = boost::winapi::DELETE_,
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
			query_limited_information = boost::winapi::PROCESS_QUERY_LIMITED_INFORMATION_,
			query_information = boost::winapi::PROCESS_QUERY_INFORMATION_,

			synchronize = boost::winapi::SYNCHRONIZE_,
		};

		enum class token
		{
			// Required to attach a primary token to a process. The SE_ASSIGNPRIMARYTOKEN_NAME privilege is also required to accomplish this task.
			assign_primary = boost::winapi::TOKEN_ASSIGN_PRIMARY_,

			// Required to duplicate an access token.
			duplicate = boost::winapi::TOKEN_DUPLICATE_,

			// Required to attach an impersonation access token to a process.
			impersonate = boost::winapi::TOKEN_DUPLICATE_,

			// Required to query an access token.
			query = boost::winapi::TOKEN_QUERY_,

			// Required to query the source of an access token.
			query_source = boost::winapi::TOKEN_QUERY_SOURCE_,

			// Required to enable or disable the privileges in an access token.
			adjust_privileges = boost::winapi::TOKEN_ADJUST_PRIVILEGES_,

			// Required to adjust the attributes of the groups in an access token.
			adjust_groups = boost::winapi::TOKEN_ADJUST_GROUPS_,

			// Required to change the default owner, primary group, or DACL of an access token.
			adjust_default = boost::winapi::TOKEN_ADJUST_DEFAULT_,

			// Required to adjust the session ID of an access token. The SE_TCB_NAME privilege is required.
			adjust_session_id = boost::winapi::TOKEN_ADJUST_SESSIONID_,

			// XX ???
			trust_constraint_mask = boost::winapi::TOKEN_TRUST_CONSTRAINT_MASK_,

			// Combines STANDARD_RIGHTS_READ and TOKEN_QUERY.
			read = boost::winapi::TOKEN_READ_,

			// Combines STANDARD_RIGHTS_WRITE, TOKEN_ADJUST_PRIVILEGES, TOKEN_ADJUST_GROUPS, and TOKEN_ADJUST_DEFAULT.
			write = boost::winapi::TOKEN_WRITE_,

			// Combines STANDARD_RIGHTS_EXECUTE and TOKEN_IMPERSONATE.
			execute = boost::winapi::TOKEN_EXECUTE_,

			// Combines all possible access rights for a token.
			all_access = boost::winapi::TOKEN_ALL_ACCESS_,
		};

		enum class page_protection
		{
			noaccess = boost::winapi::PAGE_NOACCESS_,
			readonly = boost::winapi::PAGE_READONLY_,
			writecopy = boost::winapi::PAGE_WRITECOPY_,
			guard = boost::winapi::PAGE_GUARD_,
			nocache = boost::winapi::PAGE_NOCACHE_,
			writecombine = boost::winapi::PAGE_WRITECOMBINE_,

			execute = boost::winapi::PAGE_EXECUTE_,
			execute_read = boost::winapi::PAGE_EXECUTE_READ_,
			execute_readwrite = boost::winapi::PAGE_EXECUTE_READWRITE_,
			execute_writecopy = boost::winapi::PAGE_EXECUTE_WRITECOPY_
		};


		enum class handle
		{
			inherit = boost::winapi::HANDLE_FLAG_INHERIT_,
			// Child process will inherit object handle

			close_protected = boost::winapi::HANDLE_FLAG_PROTECT_FROM_CLOSE_,
			// Prevent CloseHandle from closing handle
		};
	};

	// Define flag operators for use with conforming access_rights
	DEFINE_CONSTEXPR_ENUM_FLAG_OPERATORS(access_rights::process);
	DEFINE_CONSTEXPR_ENUM_FLAG_OPERATORS(access_rights::token);
	DEFINE_CONSTEXPR_ENUM_FLAG_OPERATORS(access_rights::standard);
	DEFINE_CONSTEXPR_ENUM_FLAG_OPERATORS(access_rights::page_protection);

	/// Check if we have permission to perform the given action
	constexpr bool check_permission(const access_rights::process given, const access_rights::process check) noexcept
	{
		return (given & check) == check;
	}

	///Check if our permission is at most \a max permission
	constexpr bool constrain_permission(const access_rights::process given, const access_rights::process max) noexcept
	{
		return check_permission(max, given);
	}

	/// Check if we have permission to perform the given action
	constexpr bool check_permission(const access_rights::token given, const access_rights::token check) noexcept
	{
		return (given & check) == check;
	}

	/// Check if we have permission to perform the given action
	constexpr bool check_permission(const access_rights::standard given, const access_rights::standard check) noexcept
	{
		return (given & check) == check;
	}

	
	/// @brief Shortcut for defining vm_operation process_rights.
	constexpr auto vm_op = access_rights::process::vm_operation;

	/// @brief Shortcut for defining vm_read process_rights.
	constexpr auto vm_read = access_rights::process::vm_read;

	/// @brief Shortcut for defining vm_write process_rights.
	constexpr auto vm_write = access_rights::process::vm_write;

	/// @brief Shortcut for defining vm_read | vm_operation process_rights.
	constexpr auto vm_r_op = vm_read | vm_op;

	/// @brief Shortcut for defining vm_write | vm_operation process_rights.
	constexpr auto vm_w_op = vm_write | vm_op;

	/// @brief Shortcut for defining vm_read | vm_write | vm_operation process_rights.
	constexpr auto vm_rw_op = vm_r_op | vm_write;

	using process_rights = access_rights::process;
	using token_rights = access_rights::token;
	using page_protection = access_rights::page_protection;
} // namespace distant
