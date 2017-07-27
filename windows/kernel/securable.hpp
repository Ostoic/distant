#pragma once

#include <distant\windows\handle.hpp>
#include <distant\windows\kernel\object.hpp>

#include <distant\type_traits.hpp>

namespace distant {
namespace windows {
namespace kernel  {

	// Could also potentially contain SECURITY_DESCRIPTOR information about the kernel object
	// This would would be CRTP/polymorphic design, in which some features are specialized by sub-types of
	// windows::kernel::securable.
	class securable : public object
	{
	public:
		using handle_type = object_traits<securable>::handle_type;

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

