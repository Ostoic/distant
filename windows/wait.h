#pragma once

#include <distant\windows\gle.h>
#include <distant\windows\kernel\object.h>

#include <distant\detail\literal.h>

#include <limits>
#include <chrono>

namespace distant {
namespace windows {

	class wait : public gle
	{
	public:
		// Windows wait codes
		enum class state
		{
			abandoned = WAIT_ABANDONED, // Wait abandoned
			signaled = WAIT_OBJECT_0,	// Object returned from wait
			timeout = WAIT_TIMEOUT,		// Wait timed out
			failed = WAIT_FAILED,		// Bad call
		};
		
	public:
		class infinitely : public detail::Literal {};

	public:
		using object_type = kernel::object;
		using time_type = DWORD;

	public:
		// Wait on kernel object
		wait::state operator ()(const object_type& obj)
		{ return this->operator()(obj, 0); }

		// Wait on kernel object until the object is done executing
		wait::state operator ()(const object_type& obj, wait::infinitely)
		{ return this->operator()(obj, INFINITE); }

		// Wait on kernel object for the given amount of time
		wait::state operator ()(const object_type& obj, time_type time)
		{
			using handle_type = object_type::handle_type;
			using value_type  = handle_type::value_type;

			auto value = obj.get_handle().get_value(); // Get handle value (void *)
			auto result = WaitForSingleObject(value, time);
			this->update_gle();

			return static_cast<state>(result);
		}
	};

	//DEFINE_ENUM_FLAG_OPERATORS(wait::state)

} // end namespace windows
} // end namespace distant