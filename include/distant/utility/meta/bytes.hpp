// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <type_traits>
#include <array>

namespace distant::utility::meta
{
	template <typename Return, std::size_t S>
	constexpr auto compose_bytes(const std::array<unsigned char, S>& array) noexcept;

	template <typename T = std::enable_if_t<std::is_pod_v<T>, T>>
	constexpr auto byte_array_from(const T&) noexcept;

	template <std::size_t N, typename T>
	constexpr unsigned char get(const T& bytes) noexcept;
} // namespace distant::utility::meta

#include "impl/bytes.hxx"
