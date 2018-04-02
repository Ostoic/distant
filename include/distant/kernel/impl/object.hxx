#pragma once
#include <distant/kernel/object.hpp>

namespace distant::kernel
{
//class object
//public:
	template <typename KernelObject, typename>
	const handle<KernelObject>& object::get_handle() const noexcept
	{
		return reinterpret_cast<const handle<KernelObject>&>(handle_);
	}

	inline const handle<object>& object::get_handle() const noexcept
	{
		return handle_;
	}

	inline object::object(handle_type&& handle) noexcept
		: handle_(std::move(handle))
		/*, error_()*/ {}

	inline object::object() noexcept
		: handle_(nullptr)
		/*, error_()*/ {}

	inline bool object::valid() const noexcept
	{
		return handle_.valid();
	}
} // end namespace distant::kernel
