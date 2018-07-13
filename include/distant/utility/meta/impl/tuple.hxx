#pragma once

#include "../tuple.hpp"

#include "../array.hpp"

#include <tuple>

namespace distant::utility::meta
{
	namespace detail
	{
		template<typename Tuple, typename Func, size_t... Is>
		constexpr void tuple_for_n_impl(Tuple&& tuple, Func fn, const std::index_sequence<Is...>) noexcept
		{
			// Unpack variadic trick
			int ignored[] = { (static_cast<void>(
				fn(std::get<Is>(std::forward<Tuple>(tuple)))
				), 0)...
			};

			(void)ignored;
		}

		template<typename Tuple, typename UnaryFunc, std::size_t... Is>
		constexpr auto tuple_transform_impl(Tuple&& tuple, UnaryFunc fn, std::index_sequence<Is...>)
		{
			using std::get;

			return std::make_tuple(
				fn(get<Is>(std::forward<Tuple>(tuple)))...
			);
		}

		template <class Tuple, std::size_t... Is>
		constexpr auto to_array_impl(Tuple&& tuple, std::index_sequence<Is...>) noexcept
		{
			using std::get;

			return make_array(
				get<Is>(std::forward<Tuple>(tuple))...
			);
		}

		template <class... Ts>
		struct common_type : std::common_type<Ts...> {};

		template <class... Ts>
		struct common_type<std::tuple<Ts...>>
		{
			using type = std::common_type_t<Ts...>;
		};

		template <class... Ts>
		struct common_type<std::array<Ts..., sizeof...(Ts)>>
		{
			using type = std::common_type_t<Ts...>;
		};

		template <class... Ts>
		using common_type_t = typename common_type<Ts...>::type;
	}

	template<class Tuple, class Func>
	constexpr void tuple_for_each(Tuple&& tuple, Func f) noexcept
	{
		std::apply(f, std::forward<Tuple>(tuple));
	}

	template<std::size_t N, class Tuple, class Func>
	constexpr void tuple_for_n(Tuple&& tuple, Func fn) noexcept
	{
		static_assert(N < std::tuple_size<Tuple>::value, "Index past the range of tuple_size");

		detail::tuple_for_n_impl(
			std::forward<Tuple>(tuple),
			fn,
			N
		);
	}

	template<typename Tuple, typename UnaryFunc>
	constexpr auto tuple_transform(Tuple&& tuple, UnaryFunc fn) noexcept
	{
		//constexpr auto size = std::tuple_size<std::decay_t<Tuple>>::value;

		return detail::tuple_transform_impl(
			std::forward<Tuple>(tuple),
			fn,
			std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{}
		);
	}

	template <class Tuple, class Value, class BinaryOp>
	constexpr Value tuple_accumulate(Tuple&& tuple, Value value, BinaryOp fn)
	{
		tuple_for_each(std::forward<Tuple>(tuple), [&value, &fn](auto&& element)
		{
			value = fn(value, std::forward<decltype(element)>(element));
		});

		return value;
	}

	template <class Tuple>
	constexpr auto to_array(Tuple&& tuple) noexcept
	{
		using result_t = detail::common_type_t<std::decay_t<Tuple>>;

		return std::apply(make_array, std::forward<Tuple>(tuple));
	}

	template <class... Ts>
	constexpr auto forward(Ts&&... ts) noexcept
	{
		return std::forward_as_tuple(std::forward<Ts>(ts));
	}

} // namespace distant::utility::meta