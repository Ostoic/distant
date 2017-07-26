#pragma once

#include <Tlhelp32.h>

#include <distant\detail\fwd.hpp>
#include <distant\type_traits.hpp>
#include <distant\windows\handle.hpp>

namespace distant {
namespace windows {
namespace system {
namespace detail  {

	template <access_rights::process access>
	windows::handle get_snapshot(const kernel::process<access>&)
	{
		auto native_handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		if (native_handle == INVALID_HANDLE_VALUE)
			return windows::invalid_handle;

		return windows::handle(native_handle);
	}

	/*windows::handle get_snapshot(tag::thread)
	{
		auto native_handle = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

		if (native_handle == INVALID_HANDLE_VALUE)
			return windows::invalid_handle;

		return windows::handle(native_handle);
	}*/

} // end namespace detail
} // end namespace system
} // end namespace windows
} // end namespace distant