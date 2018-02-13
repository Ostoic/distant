#pragma once
#include <distant/assembly/static_assembler_iterator.hpp>

namespace distant::memory
{
//class assembler_iterator:
//{ctor}:
	template <std::size_t S, std::size_t C>
	constexpr static_assembler_iterator<S, C>::static_assembler_iterator() noexcept
		: static_assembler(nullptr)
		  , index_(0) {}

	template <std::size_t S, std::size_t C>
	constexpr static_assembler_iterator<S, C>::static_assembler_iterator(const static_assembler<S, C>& assembler,
	                                                       std::size_t index) noexcept
		: index_(index)
		  , assembler_(&assembler) {}

//private: 
	template <std::size_t S, std::size_t C>
	template <std::size_t S2, std::size_t C2>
	constexpr bool static_assembler_iterator<S, C>::equal(const static_assembler_iterator<S2, C2>& other) const noexcept
	{
		return this->index_ == other.index_;
	}

	template <std::size_t S, std::size_t C>
	constexpr static_instruction<S, C> static_assembler_iterator<S, C>::dereference() const noexcept
	{
		return static_instruction<S, C>{*this->assembler_, this->assembler_->instruction_ptrs_[this->index_]};
	}

	template <std::size_t S, std::size_t C>
	void static_assembler_iterator<S, C>::increment() noexcept
	{
		++this->index_;
	}

	template <std::size_t S, std::size_t C>
	void static_assembler_iterator<S, C>::decrement() noexcept
	{
		--this->index_;
	}
}
