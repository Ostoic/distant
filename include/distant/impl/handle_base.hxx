#pragma once
#include <distant\detail\handle_base.hpp>

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <boost\winapi\handles.hpp>

namespace distant::detail {

//public:
	// Only allow native coversion via explicit cast/ctor 
	inline constexpr handle_base::handle_base(native_type h, flag_type flags, bool closed) noexcept
		: m_native_handle(h)
		, m_flags(static_cast<int>(flags)) {}

	inline constexpr handle_base::handle_base(nullptr_t) noexcept
		: handle_base(nullptr, flag_type::close_protected, false) {}

	inline constexpr handle_base::handle_base() noexcept
		: handle_base(nullptr) {}

	// Move constructor
	inline handle_base::handle_base(handle_base&& other) noexcept
		: m_native_handle(std::move(other.m_native_handle))
		, m_flags(std::move(other.m_flags))
	{ other.invalidate(); }

	// Move assignment
	inline handle_base& handle_base::operator=(handle_base&& other) noexcept
	{
		// Ensure we don't have a handle leak
		this->close();

		// Move other handle
		m_flags = std::move(other.m_flags);
		m_native_handle = std::move(other.m_native_handle);

		other.invalidate();
		return *this;
	}

	inline bool handle_base::valid() const noexcept
	{ return m_native_handle != 0; }

	inline bool handle_base::close_protected() const noexcept
	{ return m_flags == static_cast<int>(flag_type::close_protected); }

	// Test if the closed bit has been set.
	inline bool handle_base::closed() const noexcept
	{ return m_flags.test(0); }

	inline void handle_base::close() noexcept
	{
		// Close the handle if it is weakly valid and its closure wasn't observed
		if (!this->close_protected() && !this->closed() && this->valid())
		{
			boost::winapi::CloseHandle(m_native_handle);
			this->invalidate();
		}

		// Set closed bit to true.
		m_flags.set(0);
	}

//protected:
	inline void handle_base::invalidate() noexcept
	{
		this->protect();
		m_native_handle = nullptr;
	}

	inline void handle_base::protect() noexcept
	{
		m_flags.set(static_cast<int>(flag_type::close_protected), true);
		m_flags.set(static_cast<int>(flag_type::inherit), false);
	}

	inline handle_base::native_type 
	handle_base::native_handle() const noexcept
	{ return m_native_handle; }

	inline handle_base::flag_type 
	handle_base::flags() const noexcept
	{ return (m_flags.test(1))? static_cast<flag_type>(m_flags[1]) : static_cast<flag_type>(m_flags[2]); }

//free:
	// CompareObjectHandles is only available with the Windows 10 SDK or higher. 
	inline constexpr bool operator ==(const handle_base& lhs, const handle_base& rhs) noexcept
	{ return lhs.m_native_handle == rhs.m_native_handle; }

	inline constexpr bool operator !=(const handle_base& lhs, const handle_base& rhs) noexcept
	{ return !operator==(lhs, rhs); }

} // end namespace distant
