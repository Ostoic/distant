#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <cstddef>
#include <limits>

#include <string>
#include <Windows.h>

#include <distant\windows\kernel\object.hpp>
#include <distant\windows\access_rights.hpp>
#include <distant\windows\handle.hpp>

#include <distant\windows\system\process_memory.hpp>

#include <distant\utility\type_traits.hpp>

#include <distant\windows\detail\handle_service.hpp>

//#include <distant\memory\vm.h>

namespace distant::windows::kernel {
	
	// Windows process object extending a kernel securable object.
	// This is the main class to use when querying process information in 
	// this library.
	
	template <access_rights::process access_t>
	class process : public windows::kernel::object
	{
	public:
		// Object type information
		using base_type   = windows::kernel::object;

		using error_type  = typename object_traits<process>::error_type;
		using handle_type = typename object_traits<process>::handle_type;
		using access_rights = access_rights::process;

		using exit_code_type = std::size_t;

		// Process type information
		using pid_type = std::size_t;
		using flag_type = access_rights;

		//===========================//
		// Static process functions  //
		//===========================//
		// XXX Consider moving most of these into free functions
		// XXX Currently these are exposed as part of process objects.
		// XXX I believe this complicates the interface.
		// XXX Perhaps move them into an api namespace under kernel (kernel::api).
	public:

		// Get current process
		static process get_current();

		// Function used to to check if we have permission perform the given action
		static constexpr bool check_permission(flag_type access);

	protected:
		static handle_type open(pid_type);

		// XXX Implement
		static handle_type create();

		// These functions 
		//static void terminate(const handle_type&);

		//static std::string get_file_path(const handle_type&);

		//static std::size_t get_handle_count(const handle_type&);

		static pid_type get_pid(const handle_type&);

	public:
		//====================================//
		// Process id and access_rights flags //
		// accessors                          //
		//====================================//
		pid_type get_pid()	   const { return m_pid; }
		flag_type get_access() const { return m_access; }

		const windows::handle<process>& get_handle() const;

		// Check if the process handle is valid
		bool valid() const;

		void terminate() const;

		// Mutates: gle
		bool is_active() const;

		std::size_t get_handle_count() const;

		std::string get_file_path() const;

		void get_status() const;
		system::process_memory get_memory_status() const;

		// Return the virtual memory of this process
		//memory::vm get_vm() const { return memory::vm(*this); }

		// Implicitly convertible to a vm
		//operator memory::vm() const { return this->get_vm(); }

		// Idea: ostream << operator for flag, id, etc

		//=========================//
		// Process ctors and dtors //
		//=========================//
		// Empty initialize process
		constexpr process();

		// Open process by id
		explicit process(pid_type id);

		// not copy constructible/assignable
		process(const process&) = delete;
		process& operator =(const process&) = delete;

		// move constructible/assignable
		process(process&& other);
		process& operator =(process&& other);

		process(handle_type&& handle);

		// Close process handle
		// Mutates: from invalidate() 
		~process();

		template <access_rights T, access_rights U>
		friend bool operator ==(const process<T>&, const process<U>&);

		template <access_rights T, access_rights U>
		friend bool operator !=(const process<T>&, const process<U>&);

		explicit operator bool() const;

	private:
		using expose = distant::detail::attorney::to_handle<process>;
		// Close process handle and invalidate process object
		// Mutates: from invalidate() 
		void close_process();

		// Invalidate process id and handle
		// Mutates: m_id, m_handle
		void invalidate();

	protected:
		// XXX Consider containing a (lazy) list (view?) of threads inside process (so stack unwind would close thread handles first)
		// XXX Otherwise provide a function to return a reference to thread handles from this process (but the stack unwind might be problematic)
		// XXX Is it important to consider the order of closing a thread from a process? (ie close(thread); close(process) compared with the converse).
		pid_type m_pid;
		flag_type m_access;

	}; // end class process

} // end namespace distant::windows::kernel

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