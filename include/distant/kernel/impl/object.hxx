#pragma once
#include <distant\kernel\object.hpp>

namespace distant::kernel {

//public:
	template <typename KernelObject, typename>
	inline const handle<KernelObject>& object::get_handle() const noexcept
	{
		return reinterpret_cast<const handle<KernelObject>&>(m_handle);
	}

	inline const handle<object>& object::get_handle() const noexcept
	{
		return m_handle;
	}

	inline object::object(handle_type&& handle) noexcept
		: m_handle(std::move(handle)) 
		, m_error() {}

	inline object::object() noexcept 
		: m_handle(invalid_handle)
		, m_error() {}

	inline bool object::valid() const noexcept
	{
		return m_handle.valid();
	}
} // end namespace distant::kernel
