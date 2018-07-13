#pragma once

#include "../performance_info.hpp"

#include <distant/error/winapi_error.hpp>

namespace distant::system
{
	inline performance_info::performance_info()
	{
		if (!boost::winapi::get_performance_info(&info_, sizeof(info_)))
			throw winapi_error("[performance_info::{ctor}] GetPerformanceInfo failed");
	}

	inline uint performance_info::commited_pages() const
	{
		return info_.CommitTotal;
	}

	inline uint performance_info::page_limit() const
	{
		return info_.CommitLimit;
	}

	inline uint performance_info::page_peak() const
	{
		return info_.CommitPeak;
	}

	inline uint performance_info::total_physical_memory() const
	{
		return info_.PhysicalTotal;
	}

	inline uint performance_info::available_physical_memory() const
	{
		return info_.PhysicalAvailable;
	}

	inline uint performance_info::system_cache_size() const
	{
		return info_.SystemCache;
	}

	inline uint performance_info::total_kernel_memory() const
	{
		return info_.KernelTotal;
	}

	inline uint performance_info::paged_kernel_memory() const
	{
		return info_.KernelPaged;
	}

	inline uint performance_info::nonpaged_kernel_memory() const
	{
		return info_.KernelNonpaged;
	}

	inline uint performance_info::page_size() const
	{
		return info_.PageSize;
	}

	inline uint performance_info::handle_count() const
	{
		return info_.HandleCount;
	}

	inline uint performance_info::process_count() const
	{
		return info_.ProcessCount;
	}

	inline uint performance_info::thread_count() const
	{
		return info_.ThreadCount;
	}

} // namespace distant::system
