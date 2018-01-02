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

namespace distant::kernel {

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

	protected:
		static handle_type open(pid_type, access_rights_t) noexcept;

		static pid_type get_pid(const handle_type&) noexcept;

	public:
		/// Get the file name of the process executable.
		/// \return string containing the filename.
		virtual std::wstring filename() const;

		/// Get the path to the process executable.
		/// \return the path.
		virtual distant::filesystem::path file_path() const;

		/// Query the process handle to see if it is still active
		/// \return true if the process is active, false otherwise.
		virtual bool is_active() const;

		/// Terminate the process
		virtual void terminate() const;

		/// Retrieve the process id.
		/// \return the process id.
		pid_type id() const noexcept  { return m_id; }

		/// Get the access rights that were used to open the current process
		/// \return process access_rights indicating the level of access we have to the process.
		flag_type access_rights() const noexcept { return m_access_rights; }
		
		/// Test if the process object is valid
		/// \return true if the process is valid, false otherwise.
		bool valid() const noexcept override;

		/// Empty construct a process
		process_base() noexcept;

		/// Open process by id
		/// \param id the pid (process id) of the process to open.
		/// \access the requested access rights to open the process with.
		explicit process_base(pid_type id, access_rights_t access) noexcept;

		process_base(process_base&& other) noexcept; // move constructible
		process_base& operator =(process_base&& other) noexcept; // move assignable

		//process_base(const process_base&) = delete; // not copy constructible
		//process_base& operator =(const process_base&) = delete; // not copy assignable

		// TODO:
		// Return the virtual memory of this process
		//memory::vm get_vm() const { return memory::vm(*this); }

		// Implicitly convertible to a vm
		//operator memory::vm() const { return this->get_vm(); }
	private:
		using expose = distant::detail::attorney::to_handle<process_base>;

		friend bool operator ==(const process_base&, const process_base&) noexcept;
		friend bool operator !=(const process_base&, const process_base&) noexcept;

	protected:
		pid_type m_id;
		access_rights_t m_access_rights;
	}; // end class process

} // end namespace distant::kernel::detail

#include <distant\kernel\process_base.hxx>
