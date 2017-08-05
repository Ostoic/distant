#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <distant\windows\handle.hpp>

namespace distant::windows {

	template <typename T>
	inline constexpr handle<T>::handle(detail::invalid_t)
		: m_native_handle(NULL)
		, m_flags(flag_type::close_protected) // Closing invalid handle is prohibited
		, m_closed(false)
	{}

	template <typename T>
	inline constexpr handle<T>::handle(detail::null_t)
		: m_native_handle(NULL)
		, m_flags(flag_type::close_protected) // Closing null handle is prohibited
		, m_closed(false)
	{}

	template <typename T>
	inline constexpr handle<T>::handle()
		: m_native_handle(NULL)
		, m_flags(flag_type::close_protected) // Closing null handle is prohibited
		, m_closed(false)
	{}

	// Only allow conversion to underlying type through an explicit cast/ctor 
	template <typename T>
	inline constexpr handle<T>::handle(native_type h)
		: m_native_handle(h)
		, m_flags(flag_type::inherit)  // This allows the handle to be closed properly
		, m_closed(false)
	{}

	// Only allow conversion to underlying type through an explicit cast/ctor 
	template <typename T>
	inline constexpr handle<T>::handle(native_type h, flag_type flags)
		: m_native_handle(h)
		, m_flags(flags)
		, m_closed(false)
	{}

	template <typename T>
	template <typename other_t>
	inline handle<T>::handle(handle<other_t>&& other)
		: m_native_handle(std::move(other.m_native_handle))
		, m_flags(std::move(other.m_flags))
		, m_closed(std::move(other.m_closed)) 
	{
		static_assert(
			is_related<T, other_t>::value,
			"Handle object types are unrelated.");

		other.invalidate();
	}

	template <typename T>
	template <typename other_t>
	inline handle<T>& handle<T>::operator=(handle<other_t>&& other)
	{
		static_assert(
			is_related<T, other_t>::value,
			"Handle object types are unrelated.");

		m_closed = other.m_closed;
		m_flags = other.m_flags;
		m_native_handle = other.m_native_handle;
		other.invalidate();
		// other should die very soon since it is an rvalue
		return *this;
	}

	template<typename Object_t>
	template<typename other_t>
	inline handle<Object_t>::operator const handle<other_t>&() const
	{
		static_assert(
			is_related<Object_t, other_t>::value,
			"Handle object types are not related.");

		return *this;
	}

	// This weak validity should only be used for validating the handle's numeric value.
	// This does not ensure the handle is from a valid object.
	template <typename T>
	inline bool handle<T>::weakly_valid() const
	{
		return m_native_handle != NULL;
	}

	template <typename T>
	inline bool handle<T>::close_protected() const
	{
		return m_flags == flag_type::close_protected;
	}

	template <typename T>
	inline bool handle<T>::closed() const { return m_closed; }

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

} // end namespace distant::windows
