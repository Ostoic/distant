#pragma once

namespace distant::utility::meta
{
	template<class Tuple, class UnaryFunc>
	constexpr void tuple_for_each(Tuple&& tuple, UnaryFunc&& fn) ;

	template<class Tuple, class UnaryFunc>
	constexpr void tuple_for_each_reversed(Tuple&& tuple, UnaryFunc&& fn) ;

	template<std::size_t N, class Tuple, class UnaryFunc>
	constexpr void tuple_for_n(Tuple&& tuple, UnaryFunc&& fn);

	template<std::size_t N, class Tuple, class UnaryFunc>
	constexpr void tuple_for_n_reversed(Tuple&& tuple, UnaryFunc&& fn);

	template<class Tuple, class UnaryFunc>
	constexpr auto tuple_transform(Tuple&& tuple, UnaryFunc&& fn);

	template <class Tuple, class Value, class BinaryOp>
	constexpr Value tuple_accumulate(Tuple&& tuple, Value value, BinaryOp&& fn);

	template <class Tuple>
	constexpr auto to_array(Tuple&& tuple) noexcept;

	template <class Tuple>
	constexpr auto forward(Tuple&& tuple) noexcept;
}

#include "impl/tuple.hxx"
