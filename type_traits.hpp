#pragma once

#include <distant\detail\fwd.hpp>

namespace distant {
namespace windows {

	template <class Object_t>
	struct object_traits 
	{
		using handle_type = typename Object_t::handle_type;
		using error_type = typename Object_t::error_type;
	};

namespace tag {

	struct securable {};
	struct process {};
	struct object {};

	struct thread {};
}

} // end namespace windows
} // end namespace distant