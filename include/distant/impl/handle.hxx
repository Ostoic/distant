// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once
#include <distant/handle.hpp>

#include <distant/utility/asserts.hpp>

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

namespace distant
{
	//public:
	template <typename T>
	constexpr handle<T>::handle(const native_type h, const flag_type flags, const bool closed) noexcept
		: handle_base(h, flags, closed) {}

	template <typename T>
	constexpr handle<T>::handle(nullptr_t) noexcept
		: handle() {}

	template <typename T>
	constexpr handle<T>::handle() noexcept
		: handle(nullptr, flag_type::close_protected, true) {}

	// Move constructor
	template <typename T>
	template <typename OtherT>
	handle<T>::handle(handle<OtherT>&& other) noexcept
		: handle_base(std::move(other))
	{
		utility::assert_compatible<T, OtherT>();
	}

	// Move assignment
	template <typename T>
	template <typename other_t>
	handle<T>& handle<T>::operator=(handle<other_t>&& other) noexcept
	{
		utility::assert_compatible<T, other_t>();
		handle_base::operator=(std::move(other));
		return *this;
	}

	//free:
	template <typename T, typename U>
	constexpr bool operator ==(const handle<T>& lhs, const handle<U>& rhs) noexcept
	{
		// Objects must be compatible.
		// Example: thread ~/~ process, but process ~ securable
		utility::assert_compatible<T, U>();

		return operator==(
			static_cast<const detail::handle_base&>(lhs),
			static_cast<const detail::handle_base&>(rhs));
	}

	template <typename T, typename U>
	constexpr bool operator !=(const handle<T>& lhs, const handle<U>& rhs) noexcept
	{
		return !operator==(lhs, rhs);
	}
} // end namespace distant
