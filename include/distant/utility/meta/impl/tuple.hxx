#pragma once

#include "../tuple.hpp"

#include "../array.hpp"

#include <tuple>

namespace distant::utility::meta
{
	namespace detail
	{
		template<class Tuple, class UnaryFunc, size_t... Is>
		constexpr void tuple_for_n_impl(Tuple&& tuple, UnaryFunc&& fn, const std::index_sequence<Is...>)
		{
			int ignored[] = { (static_cast<void>(
				std::forward<UnaryFunc>(fn)(std::get<Is>(std::forward<Tuple>(tuple)))
				), 0)...
			};

			static_cast<void>(ignored);
		}

		template<class Tuple, class UnaryFunc, std::size_t... Is>
		constexpr auto tuple_transform_impl(Tuple&& tuple, UnaryFunc&& fn, std::index_sequence<Is...>)
		{
			using std::get;

			return std::make_tuple(
				std::forward<UnaryFunc>(fn)(get<Is>(std::forward<Tuple>(tuple)))...
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
		using common_type_t = typename common_type<Ts...>::type;
	}

	template<class Tuple, class UnaryFunc>
	constexpr void tuple_for_each_reversed(Tuple&& tuple, UnaryFunc&& fn)
	{
		tuple_for_n_reversed<std::tuple_size<remove_cvref<Tuple>>::value>(
			std::forward<Tuple>(tuple),
			std::forward<UnaryFunc>(fn)
		);
	}

	template<class Tuple, class UnaryFunc>
	constexpr void tuple_for_each(Tuple&& tuple, UnaryFunc&& fn)
	{
		tuple_for_n<std::tuple_size<remove_cvref<Tuple>>::value>(
			std::forward<Tuple>(tuple),
			std::forward<UnaryFunc>(fn)
		);
	}

	template<std::size_t N, class Tuple, class UnaryFunc>
	constexpr void tuple_for_n_reversed(Tuple&& tuple, UnaryFunc&& fn)
	{
		static_assert(N <= std::tuple_size<remove_cvref<Tuple>>::value, "Index past the range of tuple_size");

		using namespace boost::mp11;
		using reversed_indices_t = mp_to_sequence<mp_reverse<mp_from_sequence<std::make_index_sequence<N>>>>;

		detail::tuple_for_n_impl(
			std::forward<Tuple>(tuple),
			std::forward<UnaryFunc>(fn),
			reversed_indices_t{}
		);
	}

	template<std::size_t N, class Tuple, class UnaryFunc>
	constexpr void tuple_for_n(Tuple&& tuple, UnaryFunc&& fn)
	{
		static_assert(N <= std::tuple_size<remove_cvref<Tuple>>::value, "Index past the range of tuple_size");

		detail::tuple_for_n_impl(
			std::forward<Tuple>(tuple),
			std::forward<UnaryFunc>(fn),
			std::make_index_sequence<N>{}
		);
	}

	template<typename Tuple, typename UnaryFunc>
	constexpr auto tuple_transform(Tuple&& tuple, UnaryFunc&& fn)
	{
		//constexpr auto size = std::tuple_size<std::decay_t<Tuple>>::value;

		return detail::tuple_transform_impl(
			std::forward<Tuple>(tuple),
			std::forward<UnaryFunc>(fn),
			std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{}
		);
	}

	template <class Tuple, class Value, class BinaryOp>
	constexpr Value tuple_accumulate(Tuple&& tuple, Value value, BinaryOp&& fn)
	{
		tuple_for_each(std::forward<Tuple>(tuple), [&value, &fn](auto&& element)
		{
			value = std::forward<BinaryOp>(fn)(value, std::forward<decltype(element)>(element));
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