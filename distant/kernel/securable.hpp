#pragma once

#include <distant\handle.hpp>
#include <distant\kernel\object.hpp>

#include <distant\type_traits.hpp>

namespace distant {
namespace kernel  {

	// https://msdn.microsoft.com/en-us/library/windows/desktop/aa379557(v=vs.85).aspx
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
} // end namespace distant
