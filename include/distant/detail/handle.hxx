#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <distant\handle.hpp>

namespace distant {

//public:
	template <typename T>
	inline constexpr handle<T>::handle(native_type h, flag_type flags) noexcept
		: handle_base(h, flags) {}

	template <typename T>
	inline constexpr handle<T>::handle() noexcept
		: handle(NULL, flag_type::close_protected) {}

	template <typename T>
	inline constexpr handle<T>::handle(detail::invalid_t t) noexcept
		: handle() {}

	// Move constructor
	template <typename T>
	template <typename other_t>
	inline handle<T>::handle(handle<other_t>&& other) noexcept
		: handle_base(std::move(other))
	{
		static_assert(
			utility::is_related<T, other_t>::value,
			"Handle object types are unrelated.");
	}

	// Move assignment
	template <typename T>
	template <typename other_t>
	inline handle<T>& handle<T>::operator=(handle<other_t>&& other) noexcept
	{
		static_assert(
			utility::is_related<T, other_t>::value,
			"Handle object types are unrelated.");

		handle_base::operator=(std::move(other));
		return *this;
	}

//free:
	template <typename T, typename U>
	inline constexpr bool operator ==(const handle<T>& lhs, const handle<U>& rhs) noexcept
	{
		// Objects must be compatible.
		// Example: thread ~/~ process, but process ~ securable
		static_assert(
			utility::is_related<T, U>::value, // XXX Revise type check
			"Handle equality operator: Object types must be compatible");

		return operator==(
			static_cast<const detail::handle_base&>(lhs),
			static_cast<const detail::handle_base&>(rhs));
	}

	template <typename T, typename U>
	inline constexpr bool operator !=(const handle<T>& lhs, const handle<U>& rhs) noexcept
	{
		return !operator==(lhs, rhs);
	}

} // end namespace distant
