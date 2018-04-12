// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include "../memory_status.hpp"

namespace distant::kernel_objects {

//public:
	inline memory_status::memory_status(const process<required_status_access>& process)
		: memory_counters_()
	{
		if (!boost::winapi::get_process_memory_info(process.handle().native_handle(), &memory_counters_, sizeof(memory_counters_)))
			throw windows_error("[memory_status::{ctor}] K32GetProcessMemoryInfo failed");
	}

	inline std::size_t memory_status::private_usage() const noexcept
	{
		return memory_counters_.PagefileUsage / kb;
	}

	inline std::size_t memory_status::peak_private_usage() const noexcept
	{
		return memory_counters_.PeakPagefileUsage / kb;
	}

	inline std::size_t memory_status::working_set() const noexcept
	{
		return memory_counters_.WorkingSetSize / kb;
	}

	inline std::size_t memory_status::peak_working_set() const noexcept
	{
		return memory_counters_.PeakWorkingSetSize / kb;
	}

	inline std::size_t memory_status::page_fault_count() const noexcept
	{
		return memory_counters_.PageFaultCount;
	}
//end class memory_status 

} // end namespace distant::system
