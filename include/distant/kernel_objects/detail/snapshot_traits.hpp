// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/support/winapi/toolhelp32.hpp>
#include <distant/kernel_objects/thread.hpp>
#include <distant/kernel_objects/process.hpp>

namespace distant::kernel_objects::detail
{
	template <class KernelObject>

	struct snapshot_traits {};

	struct process_snapshot_traits
	{
		using entry_t = boost::winapi::PROCESSENTRY32_;

		static kernel_handle create_snapshot_handle(const boost::winapi::DWORD_ id = 0)
		{
			// Create a snapshot of all processes allowed by the primary access  token
			const auto native_handle = boost::winapi::CreateToolhelp32Snapshot(boost::winapi::TH32CS_SNAPPROCESS_, id);
			if (native_handle == boost::winapi::INVALID_HANDLE_VALUE_)
				return nullptr;

			return kernel_handle{ native_handle };
		}

		static bool first(const boost::winapi::HANDLE_ snapshot_handle, entry_t& entry) noexcept
		{
			return boost::winapi::process32_first(snapshot_handle, &entry);
		}

		// snapshot next process tag implementation
		static bool next(boost::winapi::HANDLE_ snapshot_handle, entry_t& entry) noexcept
		{
			return boost::winapi::process32_next(snapshot_handle, &entry);
		}

		// snapshot get_id process tag implementation
		static boost::winapi::DWORD_ get_id(const entry_t& entry) noexcept
		{
			return entry.th32ProcessID;
		}
	};

	struct thread_snapshot_traits
	{
		using entry_t = boost::winapi::THREADENTRY32_;

		static kernel_handle create_snapshot_handle(const boost::winapi::DWORD_ id = 0)
		{
			// Create a snapshot of all processes allowed by the primary access  token
			const auto native_handle = boost::winapi::CreateToolhelp32Snapshot(boost::winapi::TH32CS_SNAPTHREAD_, id);
			if (native_handle == boost::winapi::INVALID_HANDLE_VALUE_)
				return nullptr;

			return kernel_handle{ native_handle };
		}

		static bool first(const::boost::winapi::HANDLE_ snapshot_handle, entry_t& entry) noexcept
		{
			return boost::winapi::thread32_first(snapshot_handle, &entry);
		}

		// snapshot next process tag implementation
		static bool next(boost::winapi::HANDLE_ snapshot_handle, entry_t& entry) noexcept
		{
			return boost::winapi::thread32_next(snapshot_handle, &entry);
		}

		// snapshot get_id process tag implementation
		static boost::winapi::DWORD_ get_id(const entry_t& entry) noexcept
		{
			return entry.th32ThreadID;
		}
	};

	template <process_rights Access>
	struct snapshot_traits<process<Access>>
		: process_snapshot_traits {};

	template <>
	struct snapshot_traits<unsafe_process>
		: process_snapshot_traits {};

	template <>
	struct snapshot_traits<distant::thread>
		: thread_snapshot_traits {};

} // end namespace distant::system::detail
