#pragma once

/*!
@file Defines distant::kernel::process

@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <cstddef>
#include <string>

#include <distant\kernel\process_base.hpp>

#include <distant\utility\type_traits.hpp>

namespace distant {
namespace kernel  {
	
	/// Representation of an executable program
	template <access_rights::process access_flags>
	class process : public distant::kernel::process_base
	{
	public: // interface
		/// Terminate the process
		void kill() override;

		/// Query the process handle to see if it is still active
		/// \return true if the process is active, and false otherwise
		bool is_active() const override;

		/// Test if the process is running under the WOW64 emulator.
		/// If the process has been compiled to run on 32-bit system and
		/// is being run on a 64-bit system, it will be emulated.
		/// \return true if the process is being emulated, and false if not.
		bool is_emulated() const override;
		
		/// Get the executable name of the process
		/// \return std::wstring containing the executable name of the process
		std::wstring filename() const override;

		/// \brief Get the file path (in WIN32 format) of the process
		/// \return std::wstring containing the file path of the process
		filesystem::path file_path() const override;

		/// Query the process for memory information 
		/// \return memory_status object used to query for process information
		//auto memory_status() const;

		// Return the virtual memory of this process
		//memory::vm get_vm() const { return memory::vm(*this); }

	public: // {ctor}
		/// Default process constructor
		constexpr process() noexcept;

		/// Open process by id
		explicit process(std::size_t id) noexcept;

		process(process&& other) noexcept; /// move constructible
		process& operator =(process&& other) noexcept; /// move assignable

		explicit process(handle<process>&& handle) noexcept;

	private:
		friend class memory_status;

	}; // end class process


	/// Create a new process
	template <access_rights::process access_flag>
	process<access_flag> launch();

	/// Get the current process
	/// \return distant::process object containing the current process.
	process<> current_process() noexcept;

} // end namespace kernel

using kernel::current_process;

} // end namespace distant

// Implementation 
#include <distant\kernel\impl\process.hxx>