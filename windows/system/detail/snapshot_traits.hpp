#pragma once

#include <Tlhelp32.h>

#include <distant\windows\detail\tags.hpp>

namespace distant::windows::system::detail {
	
	template <class T>
	struct snapshot_dispatcher {};

	template <access_rights::process access>
	struct snapshot_dispatcher<kernel::process<access>>
	{
		using dispatch = windows::detail::process_tag;
		using entry_type = PROCESSENTRY32;
	};

} // end namespace distant::windows::system::detail