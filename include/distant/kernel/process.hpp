#pragma once

/*!
@file Defines distant::kernel::process

@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <cstddef>
#include <limits>
#include <string>

#include <Windows.h>
//#include <exception>

#include <distant\kernel\detail\process_base.hpp>

#include <distant\utility\type_traits.hpp>

//#include <distant\memory\vm.h>

namespace distant::kernel {
	
	/// Representation of an executable program
	template <access_rights::process access_t>
	class process : public kernel::detail::process_base
	{
	public:
		class memory_status;
		friend memory_status;

	private:
		using process_base = detail::process_base;
		using memory_status_t = memory_status;

	public:
		//===========================//
		// Static process functions  //
		//===========================//

		/// Get the process on which the code is running
		static process get_current();

		///Function used to to check if we have permission to perform the given action
		/// \param access_rights::process flag specifying the other access to check against
		/// \return true if parameter access_rights are allowable with our given access rights, 
		/// \return and false otherwise
		static constexpr bool check_permission(flag_type access);

	protected:
		// XXX Implement
		static handle<process> create();

	public:
		//===================//
		// Process interface //
		//===================//
		/// Get handle to process
		/// \return handle<process> to the process
		const handle<process>& get_handle() const;

		/// Terminate the process
		void terminate() const;

		/// Query the process handle to see if it is still active
		/// \return true if the process is active, and false otherwise
		bool is_active() const;

		/// Get the executable name of the process
		/// \return std::string containing the executable name of the process
		std::string name() const;

		/// \brief Get the file path (in WIN32 format) of the process
		/// \return std::string containing the file path of the process
		std::string file_path() const;

		/// Query the process for memory information 
		/// \return memory_status object used to query for process information
		auto memory_status() const;

		// Return the virtual memory of this process
		//memory::vm get_vm() const { return memory::vm(*this); }

		//=========================//
		// Process ctors and dtors //
		//=========================//
		/// Default process constructor
		constexpr process();

		/// Open process by id
		explicit process(pid_type id);

		process(process&& other); /// move constructible
		process& operator =(process&& other); /// move assignable

		explicit process(handle<process>&& handle);
		
		// Process destructor: Clean up handles and invalidate interior data.
		// Call Chain:
		//		1. ~process()
		//		2. ~object()
		//		3. ~handle() <-- calls CloseHandle

		template <access_rights T, access_rights U>
		friend bool operator ==(const process<T>&, const process<U>&);

		template <access_rights T, access_rights U>
		friend bool operator !=(const process<T>&, const process<U>&);

	}; // end class process

} // end namespace distant::kernel

// Implementation 
#include <distant\kernel\process\memory_status.hpp>
#include <distant\kernel\detail\process.inl>

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