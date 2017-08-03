#pragma once

/*!
@file
Includes all the library components except the adapters for external
libraries.

@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <Windows.h>

#include <distant\utility\enum_operators.hpp>

namespace distant {
namespace windows {

struct access_rights
{
	enum class process
	{
		all_access = PROCESS_ALL_ACCESS,

		set_information = PROCESS_SET_INFORMATION,
		set_quota = PROCESS_SET_QUOTA,

		vm_operation = PROCESS_VM_OPERATION,
		vm_read = PROCESS_VM_READ,
		vm_write = PROCESS_VM_WRITE,

		create_process = PROCESS_CREATE_PROCESS,
		create_thread = PROCESS_CREATE_THREAD,
		dup_handle = PROCESS_DUP_HANDLE,

		suspend_resume = PROCESS_SUSPEND_RESUME,
		terminate = PROCESS_TERMINATE,

		query_limited_information = PROCESS_QUERY_LIMITED_INFORMATION,
		query_information = PROCESS_QUERY_INFORMATION,

		synchronize = SYNCHRONIZE,
	};

	enum class handle
	{
		inherit = HANDLE_FLAG_INHERIT,					  // Child process will inherit object handle
		close_protected = HANDLE_FLAG_PROTECT_FROM_CLOSE, // Prevent CloseHandle from closing handle
	};

};

// Define flag operators for use with access_rights::process
DEFINE_CONSTEXPR_ENUM_FLAG_OPERATORS(access_rights::process);

} // end namespace windows
} // end namespace distant