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

//#include <exception>

#include <distant\kernel\object.hpp>
#include <distant\utility\type_traits.hpp>

#include <distant\support\filesystem.hpp>

//#include <distant\memory\vm.h>

namespace distant::kernel::detail {

	/// Base type of distant::process
	/// This version does not have static access_rights checking
	class process_base : public kernel::object
	{
	public:
		// Object type information
		using base_type = kernel::object;

		using error_type = typename object_traits<process_base>::error_type;
		using handle_type = typename object_traits<process_base>::handle_type;
		using access_rights_t = access_rights::process;

		using exit_code_type = std::size_t;

		// Process type information
		using pid_type = std::size_t;
		using flag_type = access_rights_t;

	public:
		//===========================//
		// Static process functions  //
		//===========================//
		// Get current process
		//static process_base get_current();

		// Function used to to check if we have permission perform the given action
		//static constexpr bool check_permission(flag_type access);

	protected:
		static handle_type open(pid_type, access_rights_t);

		static pid_type get_pid(const handle_type&);

		std::string process_base::filename() const;

		distant::filesystem::path file_path() const;

		/// Query the process handle to see if it is still active
		bool is_active() const;

		/// Terminate the process
		void terminate() const;

	public:
		//===================//
		// Process interface //
		//===================//
		pid_type id() const { return m_id; }
		flag_type access_rights() const { return m_access_rights; }
		
		//const handle<process_base>& get_handle() const;

		// Check if the process handle is valid
		bool valid() const;
		
		//void get_status() const;
		//auto memory_status() const;

		// Return the virtual memory of this process
		//memory::vm get_vm() const { return memory::vm(*this); }

		// Implicitly convertible to a vm
		//operator memory::vm() const { return this->get_vm(); }

		//=========================//
		// Process ctors and dtors //
		//=========================//
		// Empty initialize process
		constexpr process_base();

		// Open process by id
		explicit process_base(pid_type id, access_rights_t access);

		process_base(const process_base&) = delete; // not copy constructible
		process_base& operator =(const process_base&) = delete; // not copy assignable

		process_base(process_base&& other); // move constructible
		process_base& operator =(process_base&& other); // move assignable

		explicit process_base(handle_type&& handle, access_rights_t access);

		friend bool operator ==(const process_base&, const process_base&);
		friend bool operator !=(const process_base&, const process_base&);

		// For use with conditional logic to check for validity of the process
		/// Check if the process is valid or not
		/// \return true if the underlying handle is valid, and it has a valid process id
		explicit operator bool() const;

	private:
		using expose = distant::detail::attorney::to_handle<process_base>;

	protected:
		// XXX Consider containing a (lazy) list (view?) of threads inside process (so stack unwind would close thread handles first)
		// XXX Otherwise provide a function to return a reference to thread handles from this process (but the stack unwind might be problematic)
		// XXX Is it important to consider the order of closing a thread from a process? (ie close(thread); close(process) compared with the converse).
		pid_type m_id;
		access_rights_t m_access_rights;

		// Process destructor: Clean up handles and invalidate interior data.
		// Call Chain:
		//		1. ~process_base()
		//		2. ~object()
		//		3. ~handle() <-- calls CloseHandle
		~process_base() = default;

	private:
		// XXX std::string doubles the size of distant::process
		// XXX Size vs. speed tradeoff?
		//mutable std::string m_file_path_cache;

	}; // end class process

} // end namespace distant::kernel::detail

//#include <distant\kernel\process\memory_status.hpp>
#include <distant\kernel\detail\process_base.hxx>
