// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once
#include <distant/kernel_objects/kernel_object.hpp>

namespace distant::kernel_objects
{
//class kernel_object
//public:
	template <typename KernelObject, typename>
	const distant::handle<KernelObject>& kernel_object::handle() const noexcept
	{ return *reinterpret_cast<const distant::handle<KernelObject>*>(&handle_); }

	inline const distant::handle<kernel_object>& kernel_object::handle() const noexcept
	{ return handle_; }

	inline kernel_object::kernel_object(handle_type&& handle) noexcept
		: handle_(std::move(handle))
	{}

	inline kernel_object::kernel_object() noexcept
		: handle_(nullptr)
	{}

	inline bool kernel_object::valid() const noexcept
	{
		return handle_.valid();
	}
} // end namespace distant::kernel_objects
