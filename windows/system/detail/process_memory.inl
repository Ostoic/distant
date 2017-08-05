#pragma once

#include <ratio>

#include <distant\windows\system\process_memory.hpp>
#include <distant\detail\attorney.hpp>

namespace distant::windows::system {

	template <access_rights::process access>
	inline process_memory::process_memory(const windows::handle<kernel::process<access>>& handle) : m_memory_counters()
	{
		using process = kernel::process<access>;
		using get = distant::detail::attorney::to_handle<process_memory>;

		static_assert(
			process::check_permission(access_rights::vm_read) &&
				(process::check_permission(access_rights::query_information) ||
				 process::check_permission(access_rights::query_limited_information)),
			"Invalid access rights (process_memory::ctor): "
			"Process must have vm_read access right, and either query_information or query_limited_information access rights");

		auto native_handle = get::native_handle(handle);
		
		if (native_handle == NULL)
			this->set_last_error(ERROR_INVALID_HANDLE);
		else
		{
			GetProcessMemoryInfo(native_handle, &m_memory_counters, sizeof(m_memory_counters));
			this->update_gle();
		}

	}

	template <access_rights::process access>
	inline process_memory::process_memory(const kernel::process<access>& process)
		: process_memory(process.get_handle()) 
	{}

	inline std::size_t process_memory::private_usage() const
	{
		return m_memory_counters.PagefileUsage;
	}

} // end namespace distant::windows::system