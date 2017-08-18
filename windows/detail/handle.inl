#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <distant\windows\handle.hpp>

namespace distant::windows {

//public:
	// Only allow native coversion via explicit cast/ctor 
	template <typename T>
	inline constexpr handle<T>::handle(native_type h, flag_type flags)
		: handle_base(h, flags) {}

	// Only allow native coversion via explicit cast/ctor 
	template <typename T>
	inline constexpr handle<T>::handle(native_type h)
		: handle(h, flag_type::inherit) {}

	template <typename T>
	inline constexpr handle<T>::handle()
		: handle(NULL, flag_type::close_protected) {}

	template <typename T>
	inline constexpr handle<T>::handle(detail::invalid_t t)
		: handle() {}

	// Move constructor
	template <typename T>
	template <typename other_t>
	inline handle<T>::handle(handle<other_t>&& other)
		: handle_base(std::move(other))
	{
		static_assert(
			utility::is_related<T, other_t>::value,
			"Handle object types are unrelated.");
	}

	// Move assignment
	template <typename T>
	template <typename other_t>
	inline handle<T>& handle<T>::operator=(handle<other_t>&& other)
	{
		static_assert(
			utility::is_related<T, other_t>::value,
			"Handle object types are unrelated.");

		handle_base::operator=(std::move(other));
		return *this;
	}

	// Covariant handle cast
	/*template<typename Object_t>
	template<typename other_t>
	inline handle<Object_t>::operator const handle<other_t>&() const
	{
		static_assert(
			is_related<Object_t, other_t>::value,
			"Handle object types are unrelated.");

		return *this;
	}*/

//free:
	template <typename T, typename U>
	inline constexpr bool operator ==(const handle<T>& lhs, const handle<U>& rhs)
	{
		// Objects must be compatible.
		// Example: thread ~/~ process, but process ~ securable
		static_assert(
			utility::is_related<T, U>::value, // XXX Revise type check
			"Handle equality operator: Object types must be compatible");

		return handle_base::operator==(lhs, rhs);
	}

	template <typename T, typename U>
	inline constexpr bool operator !=(const handle<T>& lhs, const handle<U>& rhs)
	{
		return !operator==(lhs, rhs);
	}

} // end namespace distant::windows
