#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <cstddef>
#include <limits>

#include <string>
#include <string_view>

#include <Windows.h>
//#include <exception>

#include <distant\windows\kernel\detail\process_base.hpp>

#include <distant\utility\type_traits.hpp>

//#include <distant\memory\vm.h>

namespace distant::windows::kernel {
	
	// Windows process object extending a kernel securable object.
	// This is the main class to use when querying process information in 
	// this library.
	
	template <access_rights::process access_t>
	class process : public windows::kernel::detail::process_base
	{
	public:
		class memory_status;
		friend memory_status;

	private:
		using process_base = detail::process_base;
		using memory_status_t = memory_status;

	public:
		//using handle_type = typename object_traits<process>::handle_type;
		
	public:
		//===========================//
		// Static process functions  //
		//===========================//

		// Get current process
		static process get_current();

		// Function used to to check if we have permission perform the given action
		static constexpr bool check_permission(flag_type access);

	protected:
		// XXX Implement
		static handle_type create();

	public:
		//===================//
		// Process interface //
		//===================//
		const windows::handle<process>& get_handle() const;

		// Terminate the process
		void terminate() const;

		// Query the process handle to see if it is still active
		bool is_active() const;

		auto name() const;
		std::string file_path() const;

		//void get_status() const;
		auto memory_status() const;

		// Return the virtual memory of this process
		//memory::vm get_vm() const { return memory::vm(*this); }

		// Implicitly convertible to a vm
		//operator memory::vm() const { return this->get_vm(); }

		//=========================//
		// Process ctors and dtors //
		//=========================//
		// Empty initialize process
		constexpr process();

		// Open process by id
		explicit process(pid_type id);

		process(process&& other); // move constructible
		process& operator =(process&& other); // move assignable

		process(handle_type&& handle);

		//operator const handle_type&

		// Process destructor: Clean up handles and invalidate interior data.
		// Call Chain:
		//		1. ~process()
		//		2. ~object()
		//		3. ~handle() <-- calls CloseHandle
		~process() = default;

		template <access_rights T, access_rights U>
		friend bool operator ==(const process<T>&, const process<U>&);

		template <access_rights T, access_rights U>
		friend bool operator !=(const process<T>&, const process<U>&);

	private:
		using expose = distant::detail::attorney::to_handle<process>;

	private:
		// XXX std::string doubles the size of distant::process
		// XXX Size vs. speed tradeoff?
		//mutable std::string m_file_path_cache;

	}; // end class process

} // end namespace distant::windows::kernel

// Include implementation of memory_status
#include <distant\windows\kernel\process\memory_status.hpp>
#include <distant\windows\kernel\detail\process.inl>

/* Debug privileges for memory hacking software
BOOL EnableDebugPrivileges()
{
	HANDLE hToken;
	LUID lLuid;
	TOKEN_PRIVILEGES tkPrivileges;

	if (!(OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken)))
	{
		CloseHandle(hToken);
		return FALSE;
	}

	if (!(LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &lLuid)))
	{
		CloseHandle(hToken);
		return FALSE;
	}

	tkPrivileges.PrivilegeCount = 1;
	tkPrivileges.Privileges[0].Luid = lLuid;
	tkPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(hToken, FALSE, &tkPrivileges, sizeof(tkPrivileges), NULL, NULL))
	{
		CloseHandle(hToken);
		return FALSE;
	}

	vflog("Debug privileges enabled");
	return TRUE;
}

*/