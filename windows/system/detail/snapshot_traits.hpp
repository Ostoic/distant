#pragma once

#include <distant\detail\fwd.hpp>

namespace distant::windows::system::detail {

	class process_tag {};
	class thread_tag {};

	template <class T>
	struct snapshot_dispatcher {};

	template <access_rights::process access>
	struct snapshot_dispatcher<kernel::process<access>>
	{
		using dispatch = process_tag;
	};

} // end namespace distant::windows::system::detail