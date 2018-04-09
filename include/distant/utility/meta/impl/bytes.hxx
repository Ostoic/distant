// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once
#include "../bytes.hpp"

#include <distant/utility/meta/algorithm.hpp>

namespace distant::utility::meta
{
	template <typename Return, std::size_t S>
	constexpr auto compose_bytes(const std::array<unsigned char, S>& array) noexcept
	{
		Return result = array[0];
		for (std::size_t i = 1; i < S; ++i)
			result |= (array[i] << 8 * i);

		return result;
	}

	namespace detail
	{
		template <typename T, std::size_t... Is>
		constexpr auto byte_array_from_impl(const T& data, std::index_sequence<Is...> sequence) noexcept
		{
			return meta::make_array(meta::get<Is>(data)...);
		}
	} // namespace detail

	template <typename T>
	constexpr auto byte_array_from(const T& data) noexcept
	{
		return detail::byte_array_from_impl(data, std::make_index_sequence<sizeof(T)>{});
	}

	template <std::size_t N, typename T>
	constexpr unsigned char get(const T& bytes) noexcept
	{
		static_assert(N < sizeof(T), "[meta::get_byte] Byte index out of bounds");
		return (bytes >> (8 * N)) & 0xff;
	}
} // namespace distant::utility::meta
