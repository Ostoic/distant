// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once
#include <distant/handle.hpp>

#include <distant/utility/asserts.hpp>

namespace distant
{
	//public:
	template <typename T>
	constexpr handle<T>::handle(const native_type native_handle, const flag_type flags, const bool closed) noexcept
		: handle_base(native_handle, flags, closed) {}

	template <typename T>
	constexpr handle<T>::handle(nullptr_t) noexcept
		: handle() {}

	template <typename T>
	constexpr handle<T>::handle() noexcept
		: handle(nullptr, flag_type::close_protected, true) {}

	template <typename T>
	template <typename OtherT, typename>
	handle<T>::handle(handle<OtherT>&& other) noexcept
		: handle(std::move(*reinterpret_cast<handle*>(&other)))
	{}

	template <typename T>
	template <typename OtherT, typename>
	handle<T>& handle<T>::operator=(handle<OtherT>&& other) noexcept
	{
		return this->operator=(*std::move(reinterpret_cast<handle*>(&other)));
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
