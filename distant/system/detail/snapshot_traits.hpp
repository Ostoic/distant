#pragma once

#include <Tlhelp32.h>

#include <distant\detail\tags.hpp>

namespace distant::system::detail {
	
	template <class T>
	struct snapshot_dispatcher {};

	template <access_rights::process access>
	struct snapshot_dispatcher<kernel::process<access>>
	{
		using dispatch = distant::detail::process_tag;
		using entry_type = PROCESSENTRY32;
	};

} // end namespace distant::system::detail
