// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

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

		template<class Tuple, class Func, size_t... Is>
		void for_each_tuple_element_impl(Tuple&& tuple, Func f, const std::index_sequence<Is...>)
		{
			// Unpack variadic trick
			int ignored[] = {
				(static_cast<void>(f(
					std::get<Is>(std::forward<Tuple>(tuple))
				)), 0)...
			};

			(void)ignored;
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
	constexpr auto truncate(const std::array<T, Size>& array) noexcept
	{
		return detail::filter_array_impl(array, std::make_index_sequence<Size - 1>{});
	}

	template<class Tuple, class Func>
	void for_each_tuple(Tuple&& tuple, Func f)
	{
		detail::for_each_tuple_element_impl(
			std::forward<Tuple>(tuple),
			f,
			std::make_index_sequence<std::tuple_size<std::remove_reference_t<Tuple>>::value>()
		);
	}
} // namespacedistant::utility::meta
