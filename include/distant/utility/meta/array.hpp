// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <array>

namespace distant::utility::meta
{
	template <typename... Ts>
	constexpr auto make_array(Ts&&... ts) noexcept;

	template <typename T, std::size_t Size>
	constexpr std::array<T, Size + 1> append(const std::array<T, Size>& array, const T& value) noexcept;

	template <typename T, std::size_t Size1, std::size_t Size2>
	constexpr std::array<T, Size1 + Size2> append(const std::array<T, Size1>& array1, const std::array<T, Size2>& array2) noexcept;

	template <typename T, std::size_t Size>
	constexpr std::array<T, Size - 1> truncate(const std::array<T, Size>& array) noexcept;

	template <std::size_t Size, typename Generator>
	constexpr auto generate_array(Generator&& generator) noexcept;

	template <typename T, std::size_t Size>
	constexpr auto to_tuple(const std::array<T, Size>& array) noexcept;

} // namespace distant::utility::meta

#include "impl/array.hxx"
