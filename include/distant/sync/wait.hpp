// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/kernel_objects/kernel_object.hpp>
#include <distant/utility/literal.hpp>

#include <boost/winapi/wait.hpp>

#include <limits>
#include <chrono>
#include <utility>

namespace distant::sync 
{
	// TODO: Revise heavily
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
		class infinite : public distant::utility::literal<wait> {};

	public:
		using time_type = boost::winapi::DWORD_;

	public:
		// Wait for syncronizable object for the given amount of time
		template <typename KernelObject>
		wait::state operator ()(const KernelObject& obj, const time_type time) const
		{
			// XX Consider making free and including std::lock_guard or something
			// XX Or just the usual below 
			const auto handle = obj.handle().native_handle();
			const auto result = boost::winapi::WaitForSingleObject(handle, time);

			return static_cast<state>(result);
		}

		// Multiple object wait
		// Wait for syncronizable object for the given amount of time
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
		//		auto value = detail::attorney::to_unsafe_handle::get_value(obj.handle()); // Get handle value (void *)
		//		auto result = WaitForSingleObject(value, time);
		//	}

		//	this->update_gle();
		//	return static_cast<state>(result);
		//}

		// Wait on kernel object until the object is done executing
		template <typename KernelObject>
		wait::state operator ()(const KernelObject& obj, wait::infinite tag) const
		{ 
			static_cast<void>(tag);
			return this->operator()(obj, boost::winapi::INFINITE_); 
		}

		//wait::state operator()
	};

} // end namespace distant