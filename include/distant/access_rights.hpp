// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/utility/enum_operators.hpp>

#include <boost/winapi/access_rights.hpp>
#include <boost/winapi/handle_info.hpp>

#include <distant/support/winapi/process.hpp>
#include <distant/support/winapi/token.hpp>
#include <distant/support/winapi/thread.hpp>
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

		enum class thread
		{
			all_access = boost::winapi::THREAD_ALL_ACCESS_,

			set_information			= boost::winapi::THREAD_SET_INFORMATION_,
			set_limited_information = boost::winapi::THREAD_SET_LIMITED_INFORMATION_,

			direct_impersonation		= boost::winapi::THREAD_DIRECT_IMPERSONATION_,
			get_context					= boost::winapi::THREAD_GET_CONTEXT_,
			impersonate					= boost::winapi::THREAD_IMPERSONATE_,
			query_limited_information	= boost::winapi::THREAD_QUERY_LIMITED_INFORMATION_,
			set_context					= boost::winapi::THREAD_SET_CONTEXT_,
			set_thread_token			= boost::winapi::THREAD_SET_THREAD_TOKEN_,
			suspend_resume				= boost::winapi::THREAD_SUSPEND_RESUME_,
			terminate					= boost::winapi::THREAD_TERMINATE_,

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
			readwrite = boost::winapi::PAGE_READWRITE_,
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
			// Child process will inherit kernel_object handle

			close_protected = boost::winapi::HANDLE_FLAG_PROTECT_FROM_CLOSE_,
			// Prevent CloseHandle from closing
		};
	};

	using token_rights = access_rights::token;
	using thread_rights = access_rights::thread;
	using process_rights = access_rights::process;
	using standard_rights = access_rights::standard;
	using page_protection = access_rights::page_protection;

	// Define flag operators for use with conforming access_rights
	DEFINE_CONSTEXPR_ENUM_FLAG_OPERATORS(token_rights);
	DEFINE_CONSTEXPR_ENUM_FLAG_OPERATORS(process_rights);
	DEFINE_CONSTEXPR_ENUM_FLAG_OPERATORS(thread_rights);
	DEFINE_CONSTEXPR_ENUM_FLAG_OPERATORS(standard_rights);
	DEFINE_CONSTEXPR_ENUM_FLAG_OPERATORS(page_protection);

	DEFINE_CONSTEXPR_ENUM_FLAG_PARTIAL_ORDER(token_rights);
	DEFINE_CONSTEXPR_ENUM_FLAG_PARTIAL_ORDER(process_rights);
	DEFINE_CONSTEXPR_ENUM_FLAG_PARTIAL_ORDER(thread_rights);
	DEFINE_CONSTEXPR_ENUM_FLAG_PARTIAL_ORDER(standard_rights);
	DEFINE_CONSTEXPR_ENUM_FLAG_PARTIAL_ORDER(page_protection);

	/// Check if we have permission to perform the given action
	constexpr bool check_permission(const process_rights given, const process_rights check) noexcept
	{ return given >= check; }

	/// Check if we have permission to perform the given action
	constexpr bool check_permission(const token_rights given, const token_rights check) noexcept
	{ return (given & check) == check; }

	/// Check if we have permission to perform the given action
	constexpr bool check_permission(const standard_rights given, const standard_rights check) noexcept
	{ return (given & check) == check; }

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

	constexpr auto process_read = vm_r_op;
	constexpr auto process_write = vm_rw_op;

	// Todo: Generalize using traits and compile-time dispatch to the correct map.
	template <typename CharT, typename TraitsT>
	std::basic_ostream<CharT, TraitsT>& operator<<(std::basic_ostream<CharT, TraitsT>& stream, process_rights access);

} // namespace distant

#include "impl/access_rights.hxx"