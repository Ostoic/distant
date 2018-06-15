// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/config.hpp>
#include <distant/kernel_objects/process.hpp>
#include <distant/support/winapi/psapi.hpp>

namespace distant {
namespace kernel_objects
{
	/// @brief A class containing memory information about a process.
	class memory_status
	{
	public:
		static constexpr auto required_status_access = process_rights::vm_read | process_rights::query_limited_information;

		/// @brief Construct a status object for the given process.
		/// @remark The given process must have at least vm_read and query_limited_information access rights.
		explicit memory_status(const process<required_status_access>& process);

		/// Total amount of memory (kb) committed for the process
		/// @return the amount of memory in kilobytes.
		std::size_t private_usage() const noexcept;

		/// Largest private usage (kb) over the course its execution
		/// @return the amount of memory in kilobytes.
		std::size_t peak_private_usage() const noexcept;

		/// The size of memory (kb) occupied by the process in RAM. 
		/// See: https://en.wikipedia.org/wiki/Resident_set_size
		/// @return the amount of memory in kilobytes.
		std::size_t working_set() const noexcept;

		/// Largest working set over the course its execution
		/// @return the amount of memory in kilobytes.
		std::size_t peak_working_set() const noexcept;

		/// Number of page fault errors that have occurred over the course of its execution
		/// @return the number of page faults.
		std::size_t page_fault_count() const noexcept;

	private:
		static constexpr std::size_t kb = 1024;

		boost::winapi::PROCESS_MEMORY_COUNTERS_ memory_counters_;
	};

	/// @brief Create a status object for the given process.
	template <access_rights::process T, typename = std::enable_if_t<(T >= memory_status::required_status_access)>>
	[[nodiscard]] 
	auto get_memory_status(const process<T>& p)
	{
		return memory_status(p);
	}

} // namespace kernel_objects

using kernel_objects::get_memory_status;

} // namespace kernel_objects

// Implementation:
#include "impl/memory_status.hxx"
