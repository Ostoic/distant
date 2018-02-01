#pragma once

#include <array>

namespace distant::utility
{
	template <typename... Ts>
	constexpr auto make_array(Ts&&... ts) noexcept;

	template <typename T, std::size_t Size>
	constexpr std::array<T, Size + 1> append(const std::array<T, Size>& array, const T& value) noexcept;

	template <typename T, std::size_t Size1, std::size_t Size2>
	constexpr std::array<T, Size1 + Size2> append(const std::array<T, Size1>& array1, const std::array<T, Size2>& array2) noexcept;

	template <typename T, std::size_t Size>
	constexpr std::array<T, Size - 1> truncate(const std::array<T, Size>& array);
}

#include <distant\utility\impl\constexpr_algorithms.hxx>
