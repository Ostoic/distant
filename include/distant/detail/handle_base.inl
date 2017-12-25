#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <distant\detail\handle_base.hpp>

namespace distant::detail {

//public:
	// Only allow native coversion via explicit cast/ctor 
	inline constexpr handle_base::handle_base(native_type h, flag_type flags) noexcept
		: m_native_handle(h)
		, m_flags(flags)
		, m_closed(false) {}

	// Move constructor
	inline handle_base::handle_base(handle_base&& other) noexcept
		: m_native_handle(std::move(other.m_native_handle))
		, m_flags(std::move(other.m_flags))
		, m_closed(std::move(other.m_closed))
	{ other.invalidate(); }

	// Move assignment
	inline handle_base& handle_base::operator=(handle_base&& other) noexcept
	{
		// Ensure we don't have a handle leak
		this->close();

		// Copy other's data
		m_closed = other.m_closed;
		m_flags = other.m_flags;
		m_native_handle = other.m_native_handle;

		// Invalidate moved-from handle
		other.invalidate();
		return *this;
	}

	inline bool handle_base::valid() const noexcept
	{ return m_native_handle != NULL; }

	inline handle_base::operator bool() const noexcept
	{ return valid(); }

	inline bool handle_base::close_protected() const noexcept
	{ return m_flags == flag_type::close_protected; }

	inline bool handle_base::closed() const noexcept
	{ return m_closed; }

	// Close the handle, if it is weakly valid and its closure wasn't observed
	inline void handle_base::close() noexcept
	{
		// TODO: Query WinAPI for kernel object reference count
		// If this reference count > 0, then continue.
		// But if the reference count == 0, the system should destroy
		// the object regardless?
		if (!close_protected() && !closed() && valid())
		{
			CloseHandle(m_native_handle);
			invalidate();
		}

		m_closed = true;
	}

//protected:
	inline void handle_base::invalidate() noexcept
	{
		protect();
		m_native_handle = NULL;
	}

	inline void handle_base::protect() noexcept
	{
		m_flags = flag_type::close_protected;
	}

	inline handle_base::native_type 
	handle_base::native_handle() const noexcept
	{ return m_native_handle; }

	inline handle_base::flag_type 
	handle_base::flags() const noexcept
	{ return m_flags; }

//free:
	inline 
#if not VER_PRODUCTBUILD > 9600
		constexpr 
#endif
		bool operator ==(const handle_base& lhs, const handle_base& rhs) noexcept
	{
		return
			// CompareObjectHandles is only available with the Windows 10
			// SDK or higher. 
#if VER_PRODUCTBUILD > 9600 
			CompareObjectHandles(lhs.native_handle(), rhs.native_handle()) &&
#endif
			lhs.m_native_handle == rhs.m_native_handle;

	}

	inline
#if not VER_PRODUCTBUILD > 9600
		constexpr
#endif
		bool operator !=(const handle_base& lhs, const handle_base& rhs) noexcept
	{
		return !operator==(lhs, rhs);
	}

} // end namespace distant
