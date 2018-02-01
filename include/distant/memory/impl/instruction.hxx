#pragma once
#include <distant\memory\instruction.hpp>

#include <type_traits>

namespace distant::memory
{
//class instruction:
//{ctor}:
	template <std::size_t S, std::size_t C>
	constexpr instruction<S, C>::instruction(const assembler<S, C>& assembler, const std::pair<index_t, length_t>& ptr) noexcept
		: assembler_(assembler)
		, ptr_(ptr) {}

//interface:
	template <std::size_t S, std::size_t C>
	constexpr distant::byte instruction<S, C>::operator[](std::size_t index) const noexcept
	{
		//assert(0 <= index && index < this->ptr_.second);
		return this->assembler_.bytes_[this->ptr_.first + index];
	}

	template <std::size_t S, std::size_t C>
	constexpr std::size_t instruction<S, C>::size() const noexcept
	{ return this->ptr_.second; }
}
