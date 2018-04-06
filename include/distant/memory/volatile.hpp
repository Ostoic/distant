// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

namespace distant::memory
{
	template <typename T>
	constexpr auto remove_volatile(T&& t)
	{
		return const_cast<std::remove_volatile_t<decltype(t)>>(std::forward<T>(t));
	}
}
