#pragma once

#include "../performance_information.hpp"

#include <distant/error/windows_error.hpp>

namespace distant::system
{
	inline performance_information::performance_information()
	{
		if (!boost::winapi::get_performance_info(&info_, sizeof(info_)))
			throw windows_error("[performance_information::{ctor}] GetPerformanceInfo failed");
	}

	inline uint performance_information::commited_pages() const
	{
		return info_.CommitTotal;
	}

	inline uint performance_information::page_limit() const
	{
		return info_.CommitLimit;
	}

	inline uint performance_information::page_peak() const
	{
		return info_.CommitPeak;
	}

	inline uint performance_information::total_physical_memory() const
	{
		return info_.PhysicalTotal;
	}

	inline uint performance_information::available_physical_memory() const
	{
		return info_.PhysicalAvailable;
	}

	inline uint performance_information::system_cache_size() const
	{
		return info_.SystemCache;
	}

	inline uint performance_information::total_kernel_memory() const
	{
		return info_.KernelTotal;
	}

	inline uint performance_information::paged_kernel_memory() const
	{
		return info_.KernelPaged;
	}

	inline uint performance_information::nonpaged_kernel_memory() const
	{
		return info_.KernelNonpaged;
	}

	inline uint performance_information::page_size() const
	{
		return info_.PageSize;
	}

	inline uint performance_information::handle_count() const
	{
		return info_.HandleCount;
	}

	inline uint performance_information::process_count() const
	{
		return info_.ProcessCount;
	}

	inline uint performance_information::thread_count() const
	{
		return info_.ThreadCount;
	}

} // namespace distant::system
