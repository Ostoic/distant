#include <distant\kernel\object.hpp>

namespace distant::kernel {

//public:
	inline const handle<object>& object::get_handle() const noexcept
	{
		return m_handle;
	}

	inline object::object() noexcept 
		: m_handle(invalid_handle)
		, m_last_error(boost::winapi::NO_ERROR_) {}

	template <typename other_t>
	inline object::object(handle<other_t>&& h) noexcept
		: m_handle(std::move(h)) {}

	inline object::object(object&& other) noexcept
		: object(std::move(other.m_handle)) {}

	inline object& object::operator =(object&& other) noexcept
	{
		m_handle = std::move(other.m_handle);
		return *this;
	}

	inline object::operator bool() const noexcept
	{
		return this->valid();
	}

	inline bool object::valid() const noexcept
	{
		return m_handle.valid();
	}
} // end namespace distant::kernel
