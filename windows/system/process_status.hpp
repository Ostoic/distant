#pragma once

#include <Psapi.h>

#include <distant\windows\error\gle.hpp>
#include <distant\detail\fwd.hpp>

namespace distant::windows::system {

	// I've decided to split up process information querying into 
	// process_memory and process_status. 
	// process_status should provide access to things like cpu usage,
	// affinity, priority class, etc.

	// XXX Consider returning this object through process::get_status or some
	// XXX similar call in favor of declaring a process_memory variable freely.
	// XXX This could possibly be done through function-local class definition in
	// XXX windows::process.

	class process_status : public error::gle
	{
	private:
		using access_rights = access_rights::process;
		using error::gle::update_gle;
		using error::gle::set_last_error;

	public:
		template <access_rights access>
		explicit process_status(const kernel::process<access>& process);

		template <access_rights access>
		explicit process_status(const windows::handle<kernel::process<access>>& handle);

		std::size_t private_usage() const;

	private:
		//PROCESS_MEMORY_COUNTERS m_memory_counters;
	};

} // end namespace distant::windows::system

#include <distant\windows\system\detail\process_status.inl>