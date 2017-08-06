#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/ 

#include <distant\windows\error\gle.hpp>
#include <distant\windows\kernel\object.hpp>

#include <distant\utility\literal.hpp>
#include <distant\utility\type_traits.hpp>

#include <distant\detail\attorney.hpp>

#include <limits>
#include <chrono>
#include <utility>

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
		class infinite : public utility::Literal<wait> {};

	public:
		using object_type = kernel::object;
		using time_type = DWORD;

	public:
		// Wait for synchronizable object for the given amount of time
		wait::state operator ()(const object_type& obj, time_type time) const
		{
			using get = distant::detail::attorney::to_handle<wait>;

			auto value = get::native_handle(obj.get_handle<object_type>());
			auto result = WaitForSingleObject(value, time);
			this->update_gle();

			return static_cast<state>(result);
		}

		// Multiple object wait
		// Wait for synchronizable object for the given amount of time
		//wait::state operator ()(const std::vector<object_type>& objects, time_type time) const
		//{
		//	using handle_type = object_type::handle_type;
		//	using value_type = handle_type::value_type;

		//	const std::size_t size = objects.size();

		//	std::array<handle_type, size> handles;

		//	handle_type handles[size];

		//	for (const auto& obj : objects)
		//	{
		//		auto value = detail::attorney::to_handle<wait>::get_value(obj.get_handle()); // Get handle value (void *)
		//		auto result = WaitForSingleObject(value, time);
		//	}

		//	this->update_gle();
		//	return static_cast<state>(result);
		//}

		// Wait on kernel object until the object is done executing
		wait::state operator ()(const object_type& obj, wait::infinite tag) const
		{ 
			static_cast<void>(tag);
			return this->operator()(obj, INFINITE); 
		}

		//wait::state operator()

	};

} // end namespace windows
} // end namespace distant