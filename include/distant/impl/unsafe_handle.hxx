// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once
#include "../unsafe_handle.hpp"

#include <boost/winapi/handles.hpp>

namespace distant 
{
//public:
	// Only allow native coversion via explicit cast/ctor 
	constexpr unsafe_handle::unsafe_handle(const native_type h, flag_type flags, bool closed) noexcept
		: native_handle_(h)
		, flags_(static_cast<int>(flags)) {}

	constexpr unsafe_handle::unsafe_handle(nullptr_t) noexcept
		: unsafe_handle(nullptr, flag_type::close_protected, false) {}

	constexpr unsafe_handle::unsafe_handle() noexcept
		: unsafe_handle(nullptr) {}

	// Move constructor
	inline unsafe_handle::unsafe_handle(unsafe_handle&& other) noexcept
		: native_handle_(other.native_handle_)
		, flags_(other.flags_)
	{ other.invalidate(); }

	// Move assignment
	inline unsafe_handle& unsafe_handle::operator=(unsafe_handle&& other) noexcept
	{
		// Ensure we don't have a handle leak
		this->close();

		// Move other handle
		this->flags_ = other.flags_;
		this->native_handle_ = other.native_handle_;

		other.invalidate();
		return *this;
	}

	inline bool unsafe_handle::valid() const noexcept
	{ return native_handle_ != 0; }

	inline bool unsafe_handle::close_protected() const noexcept
	{ return flags_ == static_cast<int>(flag_type::close_protected); }

	// Test if the closed bit has been set.
	inline bool unsafe_handle::closed() const noexcept
	{ return flags_.test(0); }

	inline bool unsafe_handle::close() noexcept
	{
		// Close the handle if it is weakly valid and its closure wasn't observed
		if (!this->close_protected() && !this->closed() && this->valid())
		{
			const bool result = boost::winapi::CloseHandle(native_handle_);
			this->invalidate();
			return result;
		}

		// Set closed bit to true.
		flags_.set(0);
		return true;
	}

//protected:
	inline void unsafe_handle::invalidate() noexcept
	{
		this->protect();
		native_handle_ = nullptr;
	}

	inline void unsafe_handle::protect() noexcept
	{
		flags_.set(static_cast<int>(flag_type::close_protected), true);
		flags_.set(static_cast<int>(flag_type::inherit), false);
	}

	inline unsafe_handle::native_type 
	unsafe_handle::native_handle() const noexcept
	{ return native_handle_; }

	inline unsafe_handle::flag_type 
	unsafe_handle::flags() const noexcept
	{ return (flags_.test(1))? static_cast<flag_type>(flags_[1]) : static_cast<flag_type>(flags_[2]); }

//free:
	// CompareObjectHandles is only available with the Windows 10 SDK or higher. 
	constexpr bool operator ==(const unsafe_handle& lhs, const unsafe_handle& rhs) noexcept
	{ return lhs.native_handle_ == rhs.native_handle_; }

	constexpr bool operator !=(const unsafe_handle& lhs, const unsafe_handle& rhs) noexcept
	{ return !operator==(lhs, rhs); }

} // end namespace distant
