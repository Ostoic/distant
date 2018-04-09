// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once
#include <distant/detail/handle_base.hpp>

#include <boost/winapi/handles.hpp>

namespace distant::detail {

//public:
	// Only allow native coversion via explicit cast/ctor 
	inline constexpr handle_base::handle_base(native_type h, flag_type flags, bool closed) noexcept
		: native_handle_(h)
		, flags_(static_cast<int>(flags)) {}

	inline constexpr handle_base::handle_base(nullptr_t) noexcept
		: handle_base(nullptr, flag_type::close_protected, false) {}

	inline constexpr handle_base::handle_base() noexcept
		: handle_base(nullptr) {}

	// Move constructor
	inline handle_base::handle_base(handle_base&& other) noexcept
		: native_handle_(other.native_handle_)
		, flags_(other.flags_)
	{ other.invalidate(); }

	// Move assignment
	inline handle_base& handle_base::operator=(handle_base&& other) noexcept
	{
		// Ensure we don't have a handle leak
		this->close();

		// Move other handle
		this->flags_ = other.flags_;
		this->native_handle_ = other.native_handle_;

		other.invalidate();
		return *this;
	}

	inline bool handle_base::valid() const noexcept
	{ return native_handle_ != 0; }

	inline bool handle_base::close_protected() const noexcept
	{ return flags_ == static_cast<int>(flag_type::close_protected); }

	// Test if the closed bit has been set.
	inline bool handle_base::closed() const noexcept
	{ return flags_.test(0); }

	inline void handle_base::close() noexcept
	{
		// Close the handle if it is weakly valid and its closure wasn't observed
		if (!this->close_protected() && !this->closed() && this->valid())
		{
			boost::winapi::CloseHandle(native_handle_);
			this->invalidate();
		}

		// Set closed bit to true.
		flags_.set(0);
	}

//protected:
	inline void handle_base::invalidate() noexcept
	{
		this->protect();
		native_handle_ = nullptr;
	}

	inline void handle_base::protect() noexcept
	{
		flags_.set(static_cast<int>(flag_type::close_protected), true);
		flags_.set(static_cast<int>(flag_type::inherit), false);
	}

	inline handle_base::native_type 
	handle_base::native_handle() const noexcept
	{ return native_handle_; }

	inline handle_base::flag_type 
	handle_base::flags() const noexcept
	{ return (flags_.test(1))? static_cast<flag_type>(flags_[1]) : static_cast<flag_type>(flags_[2]); }

//free:
	// CompareObjectHandles is only available with the Windows 10 SDK or higher. 
	inline constexpr bool operator ==(const handle_base& lhs, const handle_base& rhs) noexcept
	{ return lhs.native_handle_ == rhs.native_handle_; }

	inline constexpr bool operator !=(const handle_base& lhs, const handle_base& rhs) noexcept
	{ return !operator==(lhs, rhs); }

} // end namespace distant
