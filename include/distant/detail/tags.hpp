#pragma once

#include <distant\detail\fwd.hpp>

namespace distant::detail
{
	class process_tag {};
	class thread_tag {};
	class token_tag {};
	
	template <typename KernelObject>
	struct object_dispatcher {};

	template <access_rights::process access>
	struct object_dispatcher<kernel::process<access>>
	{
		using dispatch = detail::process_tag;
	};
}