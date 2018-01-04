#pragma once

//#include <Psapi.h>

#include <distant\error\windows_error.hpp>
#include <distant\detail\fwd.hpp>

namespace distant::system {

	// I've decided to split up process information querying into 
	// process_memory and process_status. 
	// process_status should provide access to things like cpu usage,
	// affinity, priority class, etc.

	// XXX Consider returning this object through process::get_status or some
	// XXX similar call in favor of declaring a process_memory variable freely.
	// XXX This could possibly be done through function-local class definition in
	// XXX process.

	class process_status
	{
	private:
		using access_rights = access_rights::process;

	public:
		template <access_rights access>
		explicit process_status(const kernel::process<access>& process);

		template <access_rights access>
		explicit process_status(const handle<kernel::process<access>>& handle);

		std::size_t private_usage() const;

	private:
		//PROCESS_MEMORY_COUNTERS m_memory_counters;
	};

} // end namespace distant::system

//#include <distant\system\impl\process_status.hxx>
