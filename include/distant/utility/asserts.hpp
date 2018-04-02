#pragma once

#include <distant/type_traits.hpp>

namespace distant::utility
{
	template <typename T, typename U>
	constexpr void assert_compatible() noexcept
	{
		static_assert(is_quasiconvertible<T, U>::value,
			"Object types are not either direction converitble.");
	}
}
