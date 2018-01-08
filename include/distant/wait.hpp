#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/ 

#include <distant\kernel\object.hpp>
#include <boost\winapi\wait.hpp>

#include <limits>
#include <chrono>
#include <utility>

namespace distant {

	// XX Consider moving into distant::handle as a member/free function.
	// XX Look at injectory's wait interface.
	class wait
	{
	public:
		// Windows wait codes
		enum class state : long long int
		{
			abandoned = boost::winapi::WAIT_ABANDONED_, // Wait abandoned
			signaled = boost::winapi::WAIT_OBJECT_0_,	// Object returned from wait
			timeout = boost::winapi::WAIT_TIMEOUT_,		// Wait timed out
			failed = boost::winapi::WAIT_FAILED_,		// Bad call

			//io_completion = boost::winapi::WAIT_IO_COMPLETION_, // APC ended call
		};
		
	public:
		// Infinite amount of time literal
		class infinite : public distant::utility::Literal<wait> {};

	public:
		using time_type = boost::winapi::DWORD_;

	public:
		// Wait for synchronizable object for the given amount of time
		wait::state operator ()(const kernel::object& obj, time_type time) const
		{
			// XX Consider making free and including std::lock_guard or something
			// XX Or just the usual below 
			using expose = distant::detail::attorney::to_handle<wait>;

			const auto handle = obj.get_handle().native_handle();
			const auto result = boost::winapi::WaitForSingleObject(handle, time);

			return static_cast<state>(result);
		}

		// Multiple object wait
		// Wait for synchronizable object for the given amount of time
		// XXX WaitFor...Object has a particular gle syntax. Look into this.
		//wait::state operator ()(const std::vector<object_type>& objects, time_type time) const
		//{
		//	using handle_type = object_type::handle_type;
		//	using value_type = handle_type::value_type;

		//	const std::size_t size = objects.size();

		//	std::array<handle_type, size> handles;

		//	handle_type handles[size];

		//  WaitForMultipleObjects?

		//	for (const auto& obj : objects)
		//	{
		//		auto value = detail::attorney::to_handle<wait>::get_value(obj.get_handle()); // Get handle value (void *)
		//		auto result = WaitForSingleObject(value, time);
		//	}

		//	this->update_gle();
		//	return static_cast<state>(result);
		//}

		// Wait on kernel object until the object is done executing
		wait::state operator ()(const kernel::object& obj, wait::infinite tag) const
		{ 
			static_cast<void>(tag);
			return this->operator()(obj, boost::winapi::INFINITE_); 
		}

		//wait::state operator()
	private:
		mutable error::windows_error_code m_last_error;
	};

} // end namespace distant