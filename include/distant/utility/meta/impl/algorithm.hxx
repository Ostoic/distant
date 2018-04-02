#pragma once
#include "../algorithm.hpp"

#include <type_traits>

namespace distant::utility::meta
{
	namespace detail
	{
		template <typename T, std::size_t Size, std::size_t... Is>
		constexpr auto filter_array_impl(const std::array<T, Size>& array, const T& value, std::index_sequence<Is...> sequence) noexcept
		{
			return make_array(std::get<Is>(array)..., value);
		}

		template <typename T, std::size_t Size1, std::size_t Size2,
			std::size_t... I1s,
			std::size_t... I2s
		>
		constexpr std::array<T, Size1 + Size2> 
		append_impl(
			const std::array<T, Size1>& array1, const std::array<T, Size2>& array2, 
			std::index_sequence<I1s...>, std::index_sequence<I2s...>) noexcept
		{
			return make_array(std::get<I1s>(array1)..., std::get<I2s>(array2)...);
		}
	}

	template <typename... Ts>
	constexpr auto make_array(Ts&&... ts) noexcept
	{
		return std::array<std::common_type_t<Ts...>, sizeof...(Ts)>{std::forward<Ts>(ts)...};
	}

	template <typename T, std::size_t Size>
	constexpr std::array<T, Size + 1> append(const std::array<T, Size>& array, const T& value) noexcept
	{
		return detail::filter_array_impl(array, value, std::make_index_sequence<Size>{});
	}

	template <typename T, std::size_t Size1, std::size_t Size2>
	constexpr std::array<T, Size1 + Size2> append(const std::array<T, Size1>& array1, const std::array<T, Size2>& array2) noexcept
	{
		return detail::append_impl(array1, array2, std::make_index_sequence<Size1>{}, std::make_index_sequence<Size2>{});
	}

	template <typename T, std::size_t Size>
	inline constexpr auto truncate(const std::array<T, Size>& array) noexcept
	{
		return detail::filter_array_impl(array, std::make_index_sequence<Size - 1>{});
	}
} // namespacedistant::utility::meta
