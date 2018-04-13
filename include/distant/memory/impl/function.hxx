#pragma once
#include "../function.hpp"

namespace distant::memory
{
	template <typename S, typename A>
	function<S, A>::function(const memory::address<A> address)
		: address_(address)
	{}

	template <typename S, typename A>
	memory::address<A> function<S, A>::address() const noexcept
	{
		return address_;
	}
	
} // namespace distant::memory
