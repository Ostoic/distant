// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <cstddef>
#include <string>

#include <distant/config.hpp>
#include <distant/type_traits.hpp>
#include <distant/unsafe_handle.hpp>
#include <distant/support/filesystem.hpp>

namespace distant
{
	using kernel_objects::unsafe_process;

	template <>
	struct kernel_object_traits<unsafe_process>
		: default_kernel_object_traits<unsafe_process>
	{};
}

namespace distant::kernel_objects 
{
	/// @brief Base type of distant::process
	/// This version does not have static access_rights checking
	class unsafe_process : public concepts::boolean_validator<unsafe_process>
	{
	protected:
		static unsafe_handle open(std::size_t, process_rights) noexcept;

		static std::size_t get_pid(const unsafe_handle&) noexcept;

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
		std::size_t id() const noexcept { return unsafe_process::get_pid(handle_); }

		const unsafe_handle& handle() const noexcept { return handle_; }

		/// @brief Get the access rights that were used to open the current process
		/// @return process access_rights indicating the level of access we have to the process.
		process_rights access_rights() const noexcept { return access_rights_; }
		
		/// @brief Test if the process kernel_object is valid
		/// @return true if the process is valid, false otherwise.
		bool valid() const noexcept;

		/// @brief Test if the process kernel_object is valid
		/// @return true if the process is valid, false otherwise.
		explicit operator bool() const noexcept;

	public: // {ctor}
		/// @brief Construct an empty process
		unsafe_process() noexcept;

		/// @brief Open process by id
		/// @param id the pid (process id) of the process to open.
		/// @param access the requested access rights to open the process with.
		explicit unsafe_process(std::size_t id, process_rights access = process_rights::all_access) noexcept;

		unsafe_process(unsafe_process&& other) noexcept; // move constructible
		unsafe_process& operator=(unsafe_process&& other) noexcept; // move assignable

		explicit unsafe_process(unsafe_handle&& handle, process_rights) noexcept;

	private:
		friend bool operator ==(const unsafe_process&, const unsafe_process&) noexcept;
		friend bool operator !=(const unsafe_process&, const unsafe_process&) noexcept;

	protected:
		unsafe_handle handle_;
		process_rights access_rights_;
	}; // end class process

} // namespace distant::kernel_objects

#include "impl/unsafe_process.hxx"