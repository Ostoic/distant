#pragma once

namespace distant::utility::meta
{
	template<class Tuple, class Func>
	constexpr void tuple_for_each(Tuple&& tuple, Func f) noexcept;

	template<class Tuple, class Func>
	constexpr void tuple_for_n(Tuple&& tuple, Func f) noexcept;

	template<class Tuple, class Func>
	constexpr auto tuple_transform(Tuple&& tuple, Func fn) noexcept;

	template <class Tuple, class Value, class BinaryOp>
	constexpr Value tuple_accumulate(Tuple&& tuple, Value value, BinaryOp fn);

	template <class Tuple>
	constexpr auto to_array(Tuple&& tuple) noexcept;

	template <class Tuple>
	constexpr auto forward(Tuple&& tuple) noexcept;
}

#include "impl/tuple.hxx"
