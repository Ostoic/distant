// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once
#include "../unsafe_handle.hpp"

namespace distant 
{
//public:
	// Only allow native coversion via explicit cast/ctor 
	template <class C>
	constexpr unsafe_handle<C>::unsafe_handle(const native_t h, flag_t flags, const bool closed) noexcept
		: native_handle_(h)
		, flags_(static_cast<int>(flags)) {}

	template <class C>
	constexpr unsafe_handle<C>::unsafe_handle(nullptr_t) noexcept
		: unsafe_handle(nullptr, flag_t::close_protected, false) {}
	template <class C>

	constexpr unsafe_handle<C>::unsafe_handle() noexcept
		: unsafe_handle(nullptr) {}

	// Move constructor
	template <class C>
	unsafe_handle<C>::unsafe_handle(unsafe_handle&& other) noexcept
		: native_handle_(other.native_handle_)
		, flags_(other.flags_)
	{ other.invalidate(); }

	// Move assignment
	template <class C>
	unsafe_handle<C>& unsafe_handle<C>
		::operator=(unsafe_handle&& other) noexcept
	{
		// Ensure we don't have a handle leak
		this->close();

		// Move other handle
		flags_ = std::move(other.flags_);
		native_handle_ = std::move(other.native_handle_);

		other.invalidate();
		return *this;
	}

	template <class C>
	bool unsafe_handle<C>
		::valid() const noexcept
	{ return native_handle_ != 0; }

	template <class C>
	bool unsafe_handle<C>
		::close_protected() const noexcept
	{ return flags_ == static_cast<int>(flag_t::close_protected); }

	// Test if the closed bit has been set.
	template <class C>
	bool unsafe_handle<C>
		::closed() const noexcept
	{ return flags_.test(0); }

	template <class C>
	bool unsafe_handle<C>
		::close() noexcept
	{
		// Close the handle if it is weakly valid and its closure wasn't observed
		if (!this->close_protected() && !this->closed() && this->valid())
		{
			const bool result = traits_t::close(native_handle_);
			this->invalidate();
			return result;
		}

		// Set closed bit to true.
		flags_.set(0);
		return true;
	}

//protected:
	template <class C>
	void unsafe_handle<C>
		::invalidate() noexcept
	{
		this->protect();
		native_handle_ = nullptr;
	}

	template <class C>
	void unsafe_handle<C>
		::protect() noexcept
	{
		flags_.set(static_cast<int>(flag_t::close_protected), true);
		flags_.set(static_cast<int>(flag_t::inherit), false);
	}

	template <class C>
	typename unsafe_handle<C>::native_t unsafe_handle<C>
		::native_handle() const noexcept
	{ return native_handle_; }


	template <class C>
	template <typename OtherClose>
	constexpr bool unsafe_handle<C>
		::equals(const unsafe_handle<OtherClose>& other) const noexcept
	{
		return native_handle_ == other.native_handle_;
	}

	template <class C>
	typename unsafe_handle<C>::flag_t unsafe_handle<C>
		::flags() const noexcept
	{ return (flags_.test(1))? static_cast<flag_t>(flags_[1]) : static_cast<flag_t>(flags_[2]); }

} // end namespace distant
