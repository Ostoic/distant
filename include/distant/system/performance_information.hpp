#pragma once

#include <distant/support/winapi/psapi.hpp>
#include <distant/types.hpp>

namespace distant::system
{
	class performance_information
	{
	public:
		performance_information();

		/// @brief The number of pages currently committed by the system. 
		uint commited_pages() const;

		/// @brief The current maximum number of pages that can be committed by the system without extending the paging file(s).
		uint page_limit() const;

		/// @brief The maximum number of pages that were simultaneously in the committed state since the last system reboot.
		uint page_peak() const;

		/// @brief The amount of actual physical memory, in pages.
		uint total_physical_memory() const;

		/// @brief The amount of physical memory currently available, in pages. 
		/// @brief This is the amount of physical memory that can be immediately reused without having to write its contents to disk first. 
		/// @brief It is the sum of the size of the standby, free, and zero lists.
		uint available_physical_memory() const;

		/// @brief The amount of system cache memory, in pages. This is the size of the standby list plus the system working set.
		uint system_cache_size() const;

		/// @brief The sum of the memory currently in the paged and nonpaged kernel pools, in pages.
		uint total_kernel_memory() const;

		/// @brief The memory currently in the paged kernel pool, in pages.
		uint paged_kernel_memory() const;

		/// @brief The memory currently in the nonpaged kernel pool, in pages.
		uint nonpaged_kernel_memory() const;

		/// @brief The size of a page, in bytes.
		uint page_size() const;

		/// @brief The current number of open handles.
		uint handle_count() const;

		/// @brief The current number of processes.
		uint process_count() const;

		/// @brief The current number of threads.
		uint thread_count() const;

	private:
		boost::winapi::PERFORMANCE_INFORMATION_ info_;
	};
}

// impl:
#include "impl/performance_information.hxx"
