// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once
#include <distant/assembly/static_assembler_iterator.hpp>

namespace distant::assembly
{
//class assembler_iterator:
//{ctor}:
	template <std::size_t S, std::size_t C>
	constexpr static_assembler_iterator<S, C>::static_assembler_iterator() noexcept
		: static_assembler(nullptr)
		, instruction_ptrs_index_(0) 
	{}

	template <std::size_t S, std::size_t C>
	constexpr static_assembler_iterator<S, C>::static_assembler_iterator(
		const static_assembler<S, C>& assembler, 
		const index_t instruction_ptrs_index) noexcept
			: instruction_ptrs_index_(instruction_ptrs_index)
			, assembler_(&assembler) 
	{}

//private: 
	template <std::size_t S, std::size_t C>
	template <std::size_t S2, std::size_t C2>
	constexpr bool static_assembler_iterator<S, C>::equal(const static_assembler_iterator<S2, C2>& other) const noexcept
	{
		return this->instruction_ptrs_index_ == other.instruction_ptrs_index_;
	}

	template <std::size_t S, std::size_t C>
	constexpr static_instruction<S, C> static_assembler_iterator<S, C>::dereference() const noexcept
	{
		return static_instruction<S, C>{*assembler_, instruction_ptrs_index_};
	}

	template <std::size_t S, std::size_t C>
	void static_assembler_iterator<S, C>::increment() noexcept
	{
		++this->instruction_ptrs_index_;
	}

	template <std::size_t S, std::size_t C>
	void static_assembler_iterator<S, C>::decrement() noexcept
	{
		--this->instruction_ptrs_index_;
	}
}
