#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <distant\windows\handle.hpp>

namespace distant::windows {

//public:
	// Only allow native coversion via explicit cast/ctor 
	template <typename T>
	inline constexpr handle<T>::handle(native_type h, flag_type flags)
		: m_native_handle(h)
		, m_flags(flags)
		, m_closed(false) {}

	// Only allow native coversion via explicit cast/ctor 
	template <typename T>
	inline constexpr handle<T>::handle(native_type h)
		: handle(h, flag_type::inherit) {}

	template <typename T>
	inline constexpr handle<T>::handle()
		: handle(NULL, flag_type::close_protected) {}

	template <typename T>
	inline constexpr handle<T>::handle(detail::invalid_t t)
		: handle() {}

	// Move constructor
	template <typename T>
	template <typename other_t>
	inline handle<T>::handle(handle<other_t>&& other)
		: m_native_handle(std::move(other.m_native_handle))
		, m_flags(std::move(other.m_flags))
		, m_closed(std::move(other.m_closed)) 
	{
		static_assert(
			utility::is_related<T, other_t>::value,
			"Handle object types are unrelated.");

		other.invalidate();
	}

	// Move assignment
	template <typename T>
	template <typename other_t>
	inline handle<T>& handle<T>::operator=(handle<other_t>&& other)
	{
		static_assert(
			utility::is_related<T, other_t>::value,
			"Handle object types are unrelated.");

		m_closed = other.m_closed;
		m_flags = other.m_flags;
		m_native_handle = other.m_native_handle;
		other.invalidate();
		// other should die very soon since it is an rvalue
		return *this;
	}

	// Covariant handle cast
	template<typename Object_t>
	template<typename other_t>
	inline handle<Object_t>::operator const handle<other_t>&() const
	{
		static_assert(
			is_related<Object_t, other_t>::value,
			"Handle object types are not related.");

		return *this;
	}

	template <typename T>
	inline bool handle<T>::close_protected() const
	{
		return m_flags == flag_type::close_protected;
	}

	template <typename T>
	inline bool handle<T>::closed() const
	{
		return m_closed;
	}

	// This weak validity should only be used for validating the handle's numeric value.
	// This does not ensure the handle is from a valid object.
	template <typename T>
	inline bool handle<T>::weakly_valid() const
	{
		return m_native_handle != NULL;
	}

	// Close the handle, if it is weakly valid and its closure wasn't observed
	// Note: This function is public since handles occasionally need to be closed before the
	// stack unwind.
	template <typename T>
	inline void handle<T>::close_handle()
	{
		// TODO: Query WinAPI for kernel object reference count
		// If this reference count > 0, then continue.
		// But if the reference count == 0, the system should destroy
		// the object regardless?
		if (!this->close_protected() &&
			!this->closed() &&
			this->weakly_valid())
		{
			CloseHandle(m_native_handle);
			this->invalidate();
			this->update_gle();
		}

		m_closed = true;
	}

	// Numerically invalidate and close protect our handle.
	// According to "Windows Via C\C++" by Jeffrey Richter,
	// setting the handle to null is preferable to invalid_handle
	// after closing the handle. This is probably because some API
	// calls take invalid_handle as the current process.
	template <typename T>
	inline void handle<T>::invalidate()
	{
		close_protect();
		m_native_handle = NULL;
	}

	template <typename T>
	inline void handle<T>::close_protect()
	{
		m_flags = flag_type::close_protected;
	}

//free:
	template <typename T, typename U>
	inline constexpr bool operator ==(const handle<T>& lhs, const handle<U>& rhs)
	{
		// Objects must be compatible.
		// Example: thread ~/~ process, but process ~ securable
		static_assert(
			utility::is_related<T, U>::value, // XXX Revise type check
			"Handle equality operator: Object types must be compatible");

		return
			// CompareObjectHandles is only available with the Windows 10
			// SDK or higher. 
#if VER_PRODUCTBUILD > 9600 
			CompareObjectHandles(lhs.m_handle_value, rhs.m_handle_value) &&
#endif
			lhs.m_native_handle == rhs.m_native_handle;

	}

	template <typename T, typename U>
	inline constexpr bool operator !=(const handle<T>& lhs, const handle<U>& rhs)
	{
		return !operator==(lhs, rhs);
	}

} // end namespace distant::windows
