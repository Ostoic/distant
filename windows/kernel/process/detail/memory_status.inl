#pragma once

#include <distant\windows\kernel\process\memory_status.hpp>
#include <distant\detail\attorney.hpp>

namespace distant::windows::kernel {

//public:
	template <access_rights::process access_t>
	inline process<access_t>::memory_status::memory_status(const process<access_t>& process)
		: m_process(process)
		, m_memory_counters()
	{
		static_assert(
			process::check_permission(access_rights::vm_read) &&
				(process::check_permission(access_rights::query_information) ||
				 process::check_permission(access_rights::query_limited_information)),
			"Invalid access rights (process_memory::ctor): "
			"Process must have vm_read access right, and either query_information or query_limited_information access rights");

		const auto native_handle = expose::native_handle(m_process.get_handle());

		if (native_handle == NULL)
			throw std::invalid_argument(m_process.format_gle());
		else
		{
			if (!::GetProcessMemoryInfo(native_handle, &m_memory_counters, sizeof(m_memory_counters)))
				m_process.update_gle(); // Function failed, retrieve error
			else
				m_process.set_success(); // Indicate success
		}
	}

	// Total amount of memory committed for the process
	template <access_rights::process access_t>
	inline std::size_t process<access_t>::memory_status::private_usage() const
	{
		return m_memory_counters.PagefileUsage / KB;
	}

	// Largest private usage over the course its execution
	template <access_rights::process access_t>
	inline std::size_t process<access_t>::memory_status::peak_private_usage() const
	{
		return m_memory_counters.PeakPagefileUsage / KB;
	}

	// The size of memory occupied by the process in RAM. 
	template <access_rights::process access_t>
	inline std::size_t process<access_t>::memory_status::working_set() const
	{
		return m_memory_counters.WorkingSetSize / KB;
	}

	// Largest working set over the course its execution
	template <access_rights::process access_t>
	inline std::size_t process<access_t>::memory_status::peak_working_set() const
	{
		return m_memory_counters.PeakWorkingSetSize / KB;
	}

	// Number of page fault errors that have occurred over the course of its execution
	template <access_rights::process access_t>
	inline std::size_t process<access_t>::memory_status::page_fault_count() const
	{
		return m_memory_counters.PageFaultCount;
	}

	template <access_rights::process T>
	inline std::size_t process<T>::memory_status::handle_count() const
	{
		static_assert(
			check_permission(access_rights::query_information) ||
			check_permission(access_rights::query_limited_information),
			"Invalid access rights (process::get_handle_count): "
			"Process must have query_information or query_limited_information access rights");

		if (!m_process.valid())
			throw std::invalid_argument(m_process.format_gle());

		const auto native_handle = expose::native_handle(m_process.get_handle());

		DWORD count = 0;
		if (!::GetProcessHandleCount(native_handle, &count))
			m_process.update_gle();
		else
			m_process.set_success();

		return static_cast<std::size_t>(count);
	}

//end class memory_status 

} // end namespace distant::windows::system