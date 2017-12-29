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

//#include <exception>

#include <distant\kernel\process_base.hpp>

#include <distant\utility\type_traits.hpp>

//#include <distant\memory\vm.h>

namespace distant {
namespace kernel  {
	
	/// Representation of an executable program
	template <access_rights::process access_flags>
	class process : public distant::kernel::process_base
	{
	public:
		//===========================//
		// Static process functions  //
		//===========================//

		///Function used to to check if we have permission to perform the given action
		/// \param access_rights::process flag specifying the other access to check against
		/// \return true if parameter access_rights are allowable with our given access rights, 
		/// \return and false otherwise
		static constexpr bool check_permission(flag_type access) noexcept;

	public:
		//===================//
		// Process interface //
		//===================//
		/// Get handle to process
		/// \return handle<process> to the process
		//const handle<process>& get_handle() const override;

		/// Terminate the process
		void terminate() const override;

		/// Query the process handle to see if it is still active
		/// \return true if the process is active, and false otherwise
		bool is_active() const override;

		/// Get the executable name of the process
		/// \return std::string containing the executable name of the process
		std::string filename() const override;

		/// \brief Get the file path (in WIN32 format) of the process
		/// \return std::string containing the file path of the process
		distant::filesystem::path file_path() const override;

		/// Query the process for memory information 
		/// \return memory_status object used to query for process information
		//auto memory_status() const;

		// Return the virtual memory of this process
		//memory::vm get_vm() const { return memory::vm(*this); }

		/// Default process constructor
		constexpr process() noexcept;

		/// Open process by id
		explicit process(pid_type id) noexcept;

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
#include <distant\kernel\detail\process.hxx>