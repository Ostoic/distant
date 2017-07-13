#pragma once

#include <distant\windows\handle.h>
#include <distant\windows\kernel\object.h>

namespace distant {
namespace windows {
namespace kernel  {

	class securable : public object
	{
	public:
		/*********************************/
		/** Windows Object constructors **/
		/*********************************/
		// Invalid handle default constructor
		constexpr securable() : object() {}

		securable(handle_type h) : object(h) {}

		// Calls handle destructor
		~securable() {}
	};

} // end namespace kernel
} // end namespace windows
} // end namespace distant

