#pragma once

#include <type_traits>

namespace distant::utility::meta
{
	template <class T>
	using remove_cvref = std::remove_cv_t<std::remove_reference_t<T>>;


	namespace detail
	{
		template <class L>
		struct mp_to_sequence_impl;

		template <
			template <class... Cs> class L,
			template <class T2, T2 V2> class... Constants,
			class T,
			std::size_t... Vs
		>
		struct mp_to_sequence_impl<L<Constants<T, Vs>...>>
		{
			using type = std::integer_sequence<T, Vs...>;
		};
	}

	template <class L>
	using mp_to_sequence = typename detail::mp_to_sequence_impl<L>::type;
}
