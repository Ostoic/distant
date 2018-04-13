#pragma once
#include "../function.hpp"

namespace distant::memory
{
	template <typename S, typename CC, typename A>
	function<S, CC, A>::function(const memory::address<A> address)
		: address_(address)
	{}

	template <typename S, typename CC, typename A>
	memory::address<A> function<S, CC, A>::address() const noexcept
	{
		return address_;
	}
	
} // namespace distant::memory
