#pragma once

namespace distant::memory
{
	template <typename T>
	constexpr auto remove_volatile(T&& t)
	{
		return const_cast<std::remove_volatile_t<decltype(t)>>(std::forward<T>(t));
	}
}
