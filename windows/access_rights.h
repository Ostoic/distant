#pragma once

#include <Windows.h>
#include <distant\detail\enum_operators.hpp>

#include <utility>

namespace distant {
namespace windows {

enum class access_rights : int
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

// Define flag operators for use with process::access_rights
DEFINE_CONSTEXPR_ENUM_FLAG_OPERATORS(access_rights)

//constexpr bool check_permission(access_rights first, access_rights second)
//{
//	return (first & second) == second;
//}

} // end namespace windows
} // end namespace distant