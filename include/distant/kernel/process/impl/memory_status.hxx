// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/kernel/process/memory_status.hpp>
#include <distant/detail/attorney.hpp>

namespace distant::kernel {

//public:
	template <access_rights::process T>
	inline memory_status<T>::memory_status(const process<T>& process)
		: m_process(process), m_memory_counters()
	{
		using access_rights = access_rights::process;

		static_assert(
			check_permission(access_rights::vm_read) &&
				(process<T>::check_permission(access_rights::query_information) ||
				 process<T>::check_permission(access_rights::query_limited_information)),
			"Invalid access rights (memory_status::ctor): "
			"Process must have vm_read access right, and either query_information or query_limited_information access rights");

		const auto native_handle = expose::native_handle(m_process.get_handle());

		if (native_handle == NULL)
			throw std::system_error();
		else
		{
			if (!boost::winapi::get_process_memory_info(native_handle, &m_memory_counters, sizeof(m_memory_counters)))
				m_process.update_gle(); // Function failed, retrieve error
			else
				m_process.set_success(); // Indicate success
		}
	}

	template <access_rights::process T>
	inline std::size_t memory_status<T>::private_usage() const noexcept
	{
		return m_memory_counters.PagefileUsage / KB;
	}

	template <access_rights::process access_t>
	inline std::size_t memory_status<access_t>::peak_private_usage() const noexcept
	{
		return m_memory_counters.PeakPagefileUsage / KB;
	}

	template <access_rights::process access_t>
	inline std::size_t memory_status<access_t>::working_set() const noexcept
	{
		return m_memory_counters.WorkingSetSize / KB;
	}

	template <access_rights::process access_t>
	inline std::size_t process<access_t>::peak_working_set() const noexcept
	{
		return m_memory_counters.PeakWorkingSetSize / KB;
	}

	template <access_rights::process access_t>
	inline std::size_t memory_status<access_t>::page_fault_count() const noexcept
	{
		return m_memory_counters.PageFaultCount;
	}

	template <access_rights::process T>
	inline std::size_t memory_status<T>::handle_count() const
	{
		using access_rights = access_rights::process;

		static_assert(
			check_permission(access_rights::query_information) ||
			check_permission(access_rights::query_limited_information),
			"Invalid access rights (process::get_handle_count): "
			"Process must have query_information or query_limited_information access rights");

		if (!m_process.valid())
			throw std::invalid_argument(m_process.format_gle());

		const auto native_handle = expose::native_handle(m_process.get_handle());

		boost::winapi::DWORD_ count = 0;
		if (!::GetProcessHandleCount(native_handle, &count))
			m_process.update_gle();
		else
			m_process.set_success();

		return static_cast<std::size_t>(count);
	}

//end class memory_status 

} // end namespace distant::system
