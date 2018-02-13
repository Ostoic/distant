#pragma once
#include <distant/assembly/static_instruction.hpp>

namespace distant::memory
{
	//class instruction:
	//{ctor}:
	template <std::size_t S, std::size_t C>
	constexpr static_instruction<S, C>::static_instruction(const static_assembler<S, C>& assembler,
	                                                       const std::pair<index_t, length_t>& ptr) noexcept
		: assembler_(assembler)
		  , ptr_(ptr) {}

	//interface:
	template <std::size_t S, std::size_t C>
	constexpr byte static_instruction<S, C>::operator[](const std::size_t index) const noexcept
	{
		//assert(0 <= index && index < this->ptr_.second);
		return this->assembler_.bytes_[this->ptr_.first + index];
	}

	template <std::size_t S, std::size_t C>
	constexpr std::size_t static_instruction<S, C>::size() const noexcept
	{
		return this->ptr_.second;
	}
}
