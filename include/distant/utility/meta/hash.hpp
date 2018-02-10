#pragma once

#include <utility>

#include "bytes.hpp"

namespace meta
{
	template <typename T = std::enable_if_t<std::is_pod_v<T>, T>>
	struct hash
	{
		constexpr std::size_t operator()(const T& key) const noexcept;
	};
}

#include "impl\hash.hxx"
