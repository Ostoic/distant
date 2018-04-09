// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/handle.hpp>

#include <distant/type_traits.hpp>
#include <distant/kernel_objects/fwd.hpp>

#include <distant/kernel_objects/detail/snapshot_traits.hpp>

#include <distant/support/winapi/toolhelp32.hpp>

namespace distant::kernel_objects::detail {

	// get_snapshot process tag implementation
	template <typename Object_t, typename Snapshot_t>
	handle<Snapshot_t> get_snapshot_handle(distant::detail::process_tag tag) noexcept
	{
		static_cast<void>(tag);

		// Create a snapshot of all processes that we are allowed to see
		const auto native_handle = boost::winapi::CreateToolhelp32Snapshot(boost::winapi::TH32CS_SNAPPROCESS_, 0);
		if (native_handle == boost::winapi::INVALID_HANDLE_VALUE_)
			return nullptr;

		return handle<Snapshot_t>{native_handle};
	}
	
	// get_snapshot main tag dispatcher
	template <typename Object_t, typename Snapshot_t>
	handle<Snapshot_t> get_snapshot_handle() noexcept
	{
		using dispatch = typename snapshot_dispatcher<Object_t>::dispatch;
		return get_snapshot_handle<Object_t, Snapshot_t>(dispatch{});
	}

	namespace snapshot_entry 
	{
		using boost::winapi::DWORD_;
		using boost::winapi::HANDLE_;

		// snapshot first process tag implementation
		inline bool first(const boost::winapi::HANDLE_ snapshot_handle, boost::winapi::PROCESSENTRY32_* entry, distant::detail::process_tag tag) noexcept
		{
			static_cast<void>(tag);
			return boost::winapi::process32_first(snapshot_handle, entry);
		}

		// snapshot first main tag dispatcher
		template <
			typename Object_t, 
			typename Entry_t = typename snapshot_dispatcher<Object_t>::entry_type // Get snapshot entry type of kernel_objects::kernel_object
		>
		bool first(boost::winapi::HANDLE_ snapshot_handle, Entry_t& entry) noexcept
		{
			using dispatch = typename snapshot_dispatcher<Object_t>::dispatch;
			return first(snapshot_handle, &entry, dispatch{});
		}

		// snapshot next process tag implementation
		inline bool next(boost::winapi::HANDLE_ snapshot_handle, boost::winapi::PROCESSENTRY32_* entry, distant::detail::process_tag tag) noexcept
		{
			static_cast<void>(tag);
			return boost::winapi::process32_next(snapshot_handle, entry);
		}

		// snapshot next main tag dispatcher
		template <
			typename Object_t, 
			typename Entry_t = typename snapshot_dispatcher<Object_t>::entry_type // Get snapshot entry type of kernel_objects::kernel_object
		>
		bool next(boost::winapi::HANDLE_ snapshot_handle, Entry_t& entry) noexcept
		{
			using dispatch = typename snapshot_dispatcher<Object_t>::dispatch;
			return next(snapshot_handle, &entry, dispatch{});
		}

		// snapshot get_id process tag implementation
		inline boost::winapi::DWORD_ get_id(const boost::winapi::PROCESSENTRY32_& entry, distant::detail::process_tag tag) noexcept
		{
			static_cast<void>(tag);
			return entry.th32ProcessID;
		}

		// snapshot get_id main tag dispatcher
		template <
			typename Object_t, 
			typename Entry_t = typename snapshot_dispatcher<Object_t>::entry_type // Get snapshot entry type of kernel_objects::kernel_object
		>
		boost::winapi::DWORD_ get_id(const Entry_t& entry) noexcept
		{
			using dispatch = typename snapshot_dispatcher<Object_t>::dispatch;
			return get_id(entry, dispatch{});
		}

		// snapshot open_object process tag implementation
		inline HANDLE_ open_object(const DWORD_ id, const DWORD_ access, distant::detail::process_tag tag) noexcept
		{
			static_cast<void>(tag);
			return boost::winapi::OpenProcess(access, false, id);
		}

		// snapshot open_object main tag dispatcher
		template <typename Object_t>
		HANDLE_ open_object(const DWORD_ id, const DWORD_ access) noexcept
		{
			using dispatch = typename snapshot_dispatcher<Object_t>::dispatch;
			return open_object(id, access, dispatch{});
		}

	} // end namespace snapshot_entry

} // end namespace distant::system::detail
