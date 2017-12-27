#include <distant\kernel\object.hpp>

namespace distant::kernel {

//public:
	template <typename other_t>
	inline const handle<other_t>& object::get_handle() const noexcept
	{
		static_assert(
			utility::is_related<object, other_t>::value,
			"Handle types are not convertible.");

		return *reinterpret_cast<const handle<other_t>*>(&m_handle);
	}

	inline constexpr object::object() noexcept 
		: m_handle(invalid_handle) {}

	template <typename other_t>
	inline object::object(handle<other_t>&& h) 
		: m_handle(std::move(h)) {}

	inline object::object(object&& other) 
		: object(std::move(other.m_handle)) {}

	inline object& object::operator =(object&& other)
	{
		m_handle = std::move(other.m_handle);
		return *this;
	}

//protected:
	inline bool object::valid() const noexcept
	{
		return m_handle.valid();
	}

	inline void object::close_object()
	{
		m_handle.close();
	}
} // end namespace distant::kernel
