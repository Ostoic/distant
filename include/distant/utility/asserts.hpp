#pragma once

#include <distant\utility\type_traits.hpp>

namespace distant::utility
{
	template <typename T, typename U>
	constexpr void assert_compatible() noexcept
	{
		static_assert(utility::is_biconvertible<T, U>::value,
			"Object types are not convertible.");
	}
}
