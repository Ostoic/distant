// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <utility>
#include <type_traits>


namespace distant::utility::meta
{
	template <class T, class = std::enable_if_t<std::is_pod_v<T>, T>>
	struct hash
	{
		constexpr std::size_t operator()(const T& key) const noexcept;
	};

} // namespace distant::utility::meta

#include "impl/hash.hxx"
