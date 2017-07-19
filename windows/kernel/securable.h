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

		explicit securable(handle_type&& h) : 
			object(std::move(h)) 
		{}

		securable(securable&& tmp) : 
			object(std::move(tmp))
		{}

		securable& operator =(securable&& other)
		{
			object::operator=(std::move(other));
			return *this;
		}

		// Calls handle destructor
		~securable() {}
	};

} // end namespace kernel
} // end namespace windows
} // end namespace distant

