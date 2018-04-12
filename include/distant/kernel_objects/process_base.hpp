// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <cstddef>
#include <string>

#include <distant/kernel_objects/kernel_object.hpp>
#include <distant/type_traits.hpp>

#include <distant/support/filesystem.hpp>

//#include <distant\memory\vm.h>

namespace distant {
namespace kernel_objects  {

	/// @brief Base type of distant::process
	/// This version does not have static access_rights checking
	class process_base
	{
	public:
		// Object type information
		using base_type = kernel_object;

		using error_type		 = kernel_object_traits<process_base>::error_type;
		using handle_type		 = kernel_object_traits<process_base>::handle_type;
		using native_handle_type = kernel_object_traits<process_base>::native_handle_type;
		using access_rights_t	 = process_rights;

		// Process type information
		using flag_type = access_rights_t;

	protected:
		static handle_type open(std::size_t, access_rights_t) noexcept;

		static std::size_t get_pid(const handle_type&) noexcept;

	public: // interface

		/// @brief Terminate the process
		void kill();

		/// @brief Query the process handle to see if it is still active
		/// @return true if the process is active, false otherwise.
		bool is_active() const;

		/// @brief Test if the process is running under the WOW64 emulator.
		/// If the process has been compiled to run on 32-bit system and
		/// is being run on a 64-bit system, it will be emulated.
		/// @return true if the process is being emulated, and false if not.
		bool is_32bit() const;

		/// @brief Test if the process is being run in 64bit.
		/// @return true if the process is being run in 64bit mode, and false if not.
		bool is_64bit() const;

		/// @brief Get the name of the process executable.
		/// @return string containing the name.
		std::wstring name() const;

		/// @brief Get the path to the process executable.
		/// @return the path.
		filesystem::path file_path() const;

		/// @brief Test if the process is being debugged by another process.
		/// @return true if the process is being debugged, and false if it is not.
		bool is_being_debugged() const;

		/// @brief Test if the process is a "zombie" process.
		/// A zombie process in this case is one that is not listed upon process list enumeration,
		/// but still has an active handle in the operating system.
		/// @return true if the process is a zombie.
		bool is_zombie() const;

		/// Get number of handles opened in the process
		/// @return the number of handles
		std::size_t handle_count() const;

		/// @brief Retrieve the process id.
		/// @return the process id.
		std::size_t id() const noexcept { return id_; }

		const distant::handle<process_base>& handle() const noexcept { return handle_; }

		/// @brief Get the access rights that were used to open the current process
		/// @return process access_rights indicating the level of access we have to the process.
		flag_type access_rights() const noexcept { return access_rights_; }
		
		/// @brief Test if the process kernel_object is valid
		/// @return true if the process is valid, false otherwise.
		bool valid() const noexcept;

		/// @brief Test if the process kernel_object is valid
		/// @return true if the process is valid, false otherwise.
		operator bool() const noexcept;

	public: // {ctor}
		/// @brief Construct an empty process
		process_base() noexcept;

		/// @brief Open process by id
		/// @param id the pid (process id) of the process to open.
		/// @param access the requested access rights to open the process with.
		explicit process_base(std::size_t id, access_rights_t access = access_rights_t::all_access) noexcept;

		process_base(process_base&& other) noexcept; // move constructible
		process_base& operator=(process_base&& other) noexcept; // move assignable

		explicit process_base(distant::handle<process_base>&& handle, access_rights_t) noexcept;

	private:
		using expose = distant::detail::attorney::to_handle<process_base>;

		friend bool operator ==(const process_base&, const process_base&) noexcept;
		friend bool operator !=(const process_base&, const process_base&) noexcept;

		/// Throw a system_error with the given message if the process is not in a valid state.
		/// @param message the exception message to pass to system_error.
		void assert_valid(const char* message) const;
			
	protected:
		distant::handle<process_base> handle_;
		std::size_t id_;
		access_rights_t access_rights_;
	}; // end class process

} // namespace kernel_objects
} // namespace distant

#include <distant/kernel_objects/impl/process_base.hxx>
