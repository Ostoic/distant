#include <distant\windows\kernel\object.hpp>

namespace distant::windows::kernel {

//public:
	/*************************************/
	/** Windows object status functions **/
	/*************************************/
	// Get covariant-casted handle
	template <typename other_t>
	inline const windows::handle<other_t>& object::get_handle() const
	{
		static_assert(
			utility::is_related<object, other_t>::value,
			"Handle types are not convertible.");

		return *reinterpret_cast<const windows::handle<other_t>*>(&m_handle);
	}

	// Performs covariant cast to related handle type
	/*template <typename other_t>
	inline object::operator const windows::handle<other_t>&() const
	{ 
		return get_handle<other_t>(); 
	}*/

	/*********************************/
	/** Windows Object constructors **/
	/*********************************/
	// Invalid handle default constructor
	inline constexpr object::object() : m_handle(invalid_handle) {}

	template <typename other_t>
	inline object::object(windows::handle<other_t>&& h) :
		m_handle(std::move(h))
	{}

	// Move constructible/assignable.
	inline object::object(object&& other) :
		object(std::move(other.m_handle))
	{}

	inline object& object::operator =(object&& other)
	{
		m_handle = std::move(other.m_handle);
		return *this;
	}

	/*inline bool operator ==(const kernel::object& lhs, const kernel::object& rhs)
	{
		return windows::operator==(lhs.m_handle, rhs.m_handle);
	}

	inline bool operator !=(const kernel::object& lhs, const kernel::object& rhs)
	{
		return windows::operator!=(lhs.m_handle, rhs.m_handle);
	}*/

//protected:
	// Determine if the object handle is valid
	inline bool object::valid() const
	{
		return m_handle.valid();
	}

	inline void object::close_object()
	{
		m_handle.close();
	}
} // end namespace distant::windows::kernel