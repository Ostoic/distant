#pragma once

#include <Psapi.h>

#include <distant\error\gle.hpp>
#include <distant\detail\fwd.hpp>

namespace distant::kernel {

	// I've decided to split up process information querying into 
	// process_memory and process_status. 
	// process_status should provide access to things like cpu usage,
	// affinity, priority class, etc.

	// XXX Consider returning this object through process::get_status or some
	// XXX similar call in favor of declaring a process_memory variable freely.
	// XXX This could possibly be done through function-local class definition in
	// XXX process.

	// Process memory_status implemented as a nested class
	template <access_rights::process access_t>
	class process<access_t>::memory_status
	{
	public:
		explicit memory_status(const process<access_t>& process);

		// Not copy constructible
		//memory_status(const memory_status&) = delete;

		// Not copy assignable
		//memory_status& operator=(const memory_status&) = delete;
		//explicit memory_status(const handle<process<access>>& handle);

		// Total amount of memory (kb) committed for the process
		std::size_t private_usage() const;

		// Largest private usage (kb) over the course its execution
		std::size_t peak_private_usage() const;

		// The size of memory (kb) occupied by the process in RAM. 
		// See: https://en.wikipedia.org/wiki/Resident_set_size
		std::size_t working_set() const;

		// Largest working set (kb) over the course its execution
		std::size_t peak_working_set() const;

		// Number of page fault errors that have occurred over the course of its execution
		std::size_t page_fault_count() const;

		// Get number of handles opened in the process
		std::size_t handle_count() const;
		
	private:
		static constexpr std::size_t KB = 1024;

		using expose = distant::detail::attorney::to_handle<memory_status>;

		PROCESS_MEMORY_COUNTERS m_memory_counters;
		const process<access_t>& m_process;
	};

} // end namespace distant::kernel

#include <distant\kernel\process\detail\memory_status.inl>
