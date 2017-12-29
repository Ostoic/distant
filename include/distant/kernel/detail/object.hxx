#include <distant\kernel\object.hpp>

namespace distant::kernel {

//public:
	template <typename KernelObject,
		typename = std::enable_if_t<std::is_convertible<KernelObject, object>::value>>
	inline const handle<KernelObject>& object::get_handle() const noexcept
	{
		return reinterpret_cast<const handle<KernelObject>&>(m_handle);
	}

	inline const handle<object>& object::get_handle() const noexcept
	{
		return m_handle;
	}

	inline object::object() noexcept 
		: m_handle(invalid_handle)
		, m_last_error() {}

	template <typename other_t>
	inline object::object(handle<other_t>&& h) noexcept
		: m_handle(std::move(h)) {}

	inline object::operator bool() const noexcept
	{
		return this->valid();
	}

	inline bool object::valid() const noexcept
	{
		return m_handle.valid();
	}
} // end namespace distant::kernel
