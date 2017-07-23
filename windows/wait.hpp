#pragma once

/*!
@file
Includes all the library components except the adapters for external
libraries.

@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <distant\windows\error\gle.hpp>
#include <distant\windows\kernel\object.hpp>

#include <distant\utility\literal.hpp>

#include <limits>
#include <chrono>

namespace distant {
namespace windows {

	class wait : public error::gle
	{
	public:
		// Windows wait codes
		enum class state
		{
			abandoned = WAIT_ABANDONED, // Wait abandoned
			signaled = WAIT_OBJECT_0,	// Object returned from wait
			timeout = WAIT_TIMEOUT,		// Wait timed out
			failed = WAIT_FAILED,		// Bad call

			io_completion = WAIT_IO_COMPLETION, // APC ended call
		};
		
	public:
		// Infinite amount of time literal
		class infinite : public utility::Literal {};

	public:
		using object_type = kernel::object;
		using time_type = DWORD;

	public:
		// Wait on kernel object until the object is done executing
		wait::state operator ()(const object_type& obj, wait::infinite) const 
		{ return this->operator()(obj, INFINITE); }

		// Wait on kernel object for the given amount of time
		wait::state operator ()(const object_type& obj, time_type time) const
		{
			using handle_type = object_type::handle_type;
			using value_type  = handle_type::value_type;

			auto value = utility::attorney::to_handle<wait>::get_value(obj.get_handle()); // Get handle value (void *)
			auto result = WaitForSingleObject(value, time);
			this->update_gle();

			return static_cast<state>(result);
		}
	};

} // end namespace windows
} // end namespace distant