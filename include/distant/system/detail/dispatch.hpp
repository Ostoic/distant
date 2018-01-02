#pragma once

#include <distant\detail\fwd.hpp>
#include <distant\utility\type_traits.hpp>
#include <distant\handle.hpp>

#include <distant\system\detail\snapshot_traits.hpp> // Includes <Tlhelp32.h>

namespace distant::system::detail {

	// get_snapshot process tag implementation
	template <typename Object_t, typename Snapshot_t>
	inline handle<Snapshot_t> get_snapshot(distant::detail::process_tag tag)
	{
		static_cast<void>(tag);

		// Create a snapshot of all processes that we are allowed to see
		const auto native_handle = boost::winapi::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (native_handle == INVALID_HANDLE_VALUE)
			return invalid_handle;

		return handle<Snapshot_t>(native_handle);
	}
	
	// get_snapshot main tag dispatcher
	// We templated Snapshot_t because we are lazy.
	template <typename Object_t, typename Snapshot_t>
	inline handle<Snapshot_t> get_snapshot()
	{
		using dispatch = typename snapshot_dispatcher<Object_t>::dispatch;
		return get_snapshot<Object_t, Snapshot_t>(dispatch());
	}

	namespace snapshot_entry 
	{
		// snapshot_first process tag implementation
		inline bool first(boost::winapi::HANDLE_ native_handle, boost::winapi::PROCESSENTRY32_* entry, distant::detail::process_tag tag)
		{
			static_cast<void>(tag);
			return boost::winapi::process32_first(native_handle, entry);
		}

		// snapshot_first main tag dispatcher
		template <
			typename Object_t, 
			typename Entry_t = snapshot_dispatcher<Object_t>::entry_type // Get snapshot entry type of kernel::object
		>
		inline bool first(boost::winapi::HANDLE_ native_handle, Entry_t* entry)
		{
			using dispatch = typename snapshot_dispatcher<Object_t>::dispatch;
			return first(native_handle, entry, dispatch());
		}

		// snapshot_next process tag implementation
		inline bool next(boost::winapi::HANDLE_ native_handle, boost::winapi::PROCESSENTRY32_* entry, distant::detail::process_tag tag)
		{
			static_cast<void>(tag);
			return boost::winapi::process32_next(native_handle, entry);
		}

		// snapshot_next main tag dispatcher
		template <
			typename Object_t, 
			typename Entry_t = snapshot_dispatcher<Object_t>::entry_type // Get snapshot entry type of kernel::object
		>
		inline bool next(boost::winapi::HANDLE_ native_handle, Entry_t* entry)
		{
			using dispatch = typename snapshot_dispatcher<Object_t>::dispatch;
			return next(native_handle, entry, dispatch());
		}

		// snapshot_next process tag implementation
		inline boost::winapi::DWORD_ get_id(const boost::winapi::PROCESSENTRY32_& entry, distant::detail::process_tag tag)
		{
			static_cast<void>(tag);
			return entry.th32ProcessID;
		}

		// snapshot_next main tag dispatcher
		template <
			typename Object_t, 
			typename Entry_t = snapshot_dispatcher<Object_t>::entry_type // Get snapshot entry type of kernel::object
		>
		inline boost::winapi::DWORD_ get_id(const Entry_t& entry)
		{
			using dispatch = typename snapshot_dispatcher<Object_t>::dispatch;
			return get_id(entry, dispatch());
		}

	} // end namespace snapshot_entry

} // end namespace distant::system::detail
