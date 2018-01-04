#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <distant\detail\handle_base.hpp>
#include <boost\winapi\handles.hpp>

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

		// Move other handle
		m_closed = other.m_closed;
		m_flags = other.m_flags;
		m_native_handle = other.m_native_handle;

		other.invalidate();
		return *this;
	}

	inline bool handle_base::valid() const noexcept
	{ return m_native_handle != nullptr; }

	inline handle_base::operator bool() const noexcept
	{ return valid(); }

	inline bool handle_base::close_protected() const noexcept
	{ return m_flags == flag_type::close_protected; }

	inline bool handle_base::closed() const noexcept
	{ return m_closed; }

	inline void handle_base::close() noexcept
	{
		// Close the handle if it is weakly valid and its closure wasn't observed
		if (!this->close_protected() && !this->closed() && this->valid())
		{
			boost::winapi::CloseHandle(m_native_handle);
			this->invalidate();
		}

		m_closed = true;
	}

//protected:
	inline void handle_base::invalidate() noexcept
	{
		this->protect();
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
#if BOOST_USE_WINAPI_VERSION < BOOST_WINAPI_VERSION_WIN10
		constexpr 
#endif
	bool operator ==(const handle_base& lhs, const handle_base& rhs) noexcept
	{
		return
			// CompareObjectHandles is only available with the Windows 10
			// SDK or higher. 
#if BOOST_USE_WINAPI_VERSION >= BOOST_WINAPI_VERSION_WIN10
			//CompareObjectHandles(lhs.native_handle(), rhs.native_handle()) &&
#endif
			lhs.m_native_handle == rhs.m_native_handle;

	}

	inline
#if BOOST_USE_WINAPI_VERSION < BOOST_WINAPI_VERSION_WIN10
		constexpr
#endif
	bool operator !=(const handle_base& lhs, const handle_base& rhs) noexcept
	{
		return !operator==(lhs, rhs);
	}

} // end namespace distant
