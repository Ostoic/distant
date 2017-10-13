#pragma once

#include <distant\detail\fwd.hpp>

namespace distant::windows::detail
{
	class process_tag {};
	class thread_tag {};
	class token_tag {};
	
	template <typename KernelObject>
	struct object_dispatcher {};

	template <access_rights::process access>
	struct object_dispatcher<kernel::process<access>>
	{
		using dispatch = windows::detail::process_tag;
	};
}