// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/detail/fwd.hpp>
#include <distant/error/windows_error.hpp>

#include <distant/support/winapi/psapi.hpp>

#include <distant/kernel/process_base.hpp>

namespace distant::kernel {

	// I've decided to split up process information querying into 
	// process_memory and process_status. 
	// process_status should provide access to things like cpu usage,
	// affinity, priority class, etc.

	// XXX Consider returning this object through process::get_status or some
	// XXX similar call in favor of declaring a process_memory variable freely.
	// XXX This could possibly be done through function-local class definition in
	// XXX process.

	class memory_status
	{
	public:
		explicit memory_status(const process_base& process);

		/// Total amount of memory (kb) committed for the process
		/// @return the amount of memory in kilobytes.
		std::size_t private_usage() const noexcept;

		/// Largest private usage (kb) over the course its execution
		/// @return the amount of memory in kilobytes.
		std::size_t peak_private_usage() const noexcept;

		/// The size of memory (kb) occupied by the process in RAM. 
		/// See: https://en.wikipedia.org/wiki/Resident_set_size
		/// @return the amount of memory in kilobytes.
		std::size_t working_set() const noexcept;

		/// Largest working set over the course its execution
		/// @return the amount of memory in KB.
		std::size_t peak_working_set() const noexcept;

		/// Number of page fault errors that have occurred over the course of its execution
		/// @return the number of page faults.
		std::size_t page_fault_count() const noexcept;

		/// Get number of handles opened in the process
		/// @return the number of handles
		std::size_t handle_count() const;
		
	private:
		static constexpr std::size_t KB = 1024;

		using expose = distant::detail::attorney::to_handle<memory_status>;

		boost::winapi::PROCESS_MEMORY_COUNTERS_ m_memory_counters;
		const process<access_t>& m_process;
	};
	
	template <access_rights::process T>
	inline auto get_memory_status(const process<T>& p)
	{
		using access_rights = distant::access_rights::process;
		static_assert(
			check_permission(T, access_rights::vm_read))
			"Invalid access rights (status::ctor): "
			"Process must have vm_read access right, and either query_information or query_limited_information access rights"); "
			
		);
		return memory_status<T>(p);
	}

} // end namespace distant::kernel

// Implementation:
#include <distant/kernel/process/impl/memory_status.hxx>
