#pragma once

#include <Tlhelp32.h>

#include <distant\detail\fwd.hpp>
#include <distant\type_traits.hpp>
#include <distant\windows\handle.hpp>

#include <distant\windows\system\detail\snapshot_traits.hpp>

namespace distant::windows::system::detail {

	template <typename Object_t, typename Snapshot_t>
	inline windows::handle<Snapshot_t> get_snapshot(process_tag tag)
	{
		// Create a snapshot of all processes that we are allowed to see
		auto native_handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (native_handle == INVALID_HANDLE_VALUE)
			return windows::invalid_handle;

		return windows::handle<Snapshot_t>(native_handle);
	}
	
	// We templated Snapshot_t because we are lazy.
	template <typename Object_t, typename Snapshot_t>
	inline windows::handle<Snapshot_t> get_snapshot()
	{
		using dispatch = typename snapshot_dispatcher<Object_t>::dispatch;
		return get_snapshot<Object_t, Snapshot_t>(dispatch());
	}

	/*windows::handle get_snapshot(tag::thread)																			
	{
		auto native_handle = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

		if (native_handle == INVALID_HANDLE_VALUE)
			return windows::invalid_handle;

		return windows::handle(native_handle);
	}*/

} // end namespace distant::windows::system::detail