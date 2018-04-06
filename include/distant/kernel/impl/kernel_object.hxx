// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once
#include <distant/kernel/kernel_object.hpp>

namespace distant::kernel
{
//class kernel_object
//public:
	template <typename KernelObject, typename>
	const handle<KernelObject>& kernel_object::get_handle() const noexcept
	{ return reinterpret_cast<const handle<KernelObject>&>(handle_); }

	inline const handle<kernel_object>& kernel_object::get_handle() const noexcept
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
} // end namespace distant::kernel
