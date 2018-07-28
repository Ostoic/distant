// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once
#include "../scoped_handle.hpp"

namespace distant
{
//public:
	// Only allow native coversion via explicit cast/ctor
	template <class C>
	constexpr scoped_handle<C>::scoped_handle(const native_t h, const bool is_closed) noexcept
		: native_handle_(h)
		, closed_(is_closed) {}

	template <class C>
	constexpr scoped_handle<C>::scoped_handle(nullptr_t) noexcept
		: scoped_handle(nullptr, true) {}

	template <class C>
	constexpr scoped_handle<C>::scoped_handle() noexcept
		: scoped_handle(nullptr) {}

	// Move constructor
	template <class C>
	scoped_handle<C>::scoped_handle(scoped_handle&& other) noexcept
		: native_handle_(std::move(other.native_handle_))
		, closed_(other.closed_)
	{ other.closed_ = true; }

	// Move assignment
	template <class C>
	scoped_handle<C>& scoped_handle<C>
		::operator=(scoped_handle&& other) noexcept
	{
		// Ensure we don't have a handle leak
		this->close();

		// Move other handle
		closed_ = std::move(other.closed_);
		native_handle_ = std::move(other.native_handle_);
		other.closed_ = true;
		return *this;
	}

	template <class C>
	bool scoped_handle<C>
		::valid() const noexcept
	{ return native_handle_ != nullptr; }

	// Test if the closed bit has been set.
	template <class C>
	bool scoped_handle<C>
		::is_closed() const noexcept
	{ return closed_; }

	template <class C>
	bool scoped_handle<C>
		::close() noexcept
	{
		// Close the handle if it is weakly valid and its closure wasn't observed
		if (!this->is_closed() && this->valid())
			native_handle_.reset();

		// Set closed bit to true.
		closed_ = true;
		return true;
	}

//protected:
	template <class C>
	constexpr typename scoped_handle<C>::native_t scoped_handle<C>
		::native_handle() const noexcept
	{ return native_handle_.get(); }

} // end namespace distant
