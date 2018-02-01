#pragma once
#include <distant\memory\assembler_iterator.hpp>

namespace distant::memory
{
//class assembler_iterator:
//{ctor}:
	template <std::size_t S, std::size_t C>
	constexpr assembler_iterator<S, C>::assembler_iterator() noexcept
		: assembler(nullptr)
		, index_(0) {}

	template <std::size_t S, std::size_t C>
	constexpr assembler_iterator<S, C>::assembler_iterator(const assembler<S, C>& assembler, std::size_t index) noexcept
		: assembler_(&assembler)
		, index_(index) {}

//private: 
	template <std::size_t S, std::size_t C>
	template <std::size_t S2, std::size_t C2>
	constexpr bool assembler_iterator<S, C>::equal(const assembler_iterator<S2, C2>& other) const noexcept
	{ return this->index_ == other.index_; }

	template <std::size_t S, std::size_t C>
	constexpr instruction<S, C> assembler_iterator<S, C>::dereference() const noexcept
	{
		return instruction<S, C>{*this->assembler_, this->assembler_->instruction_ptrs_[this->index_]};
	}

	template <std::size_t S, std::size_t C>
	void assembler_iterator<S, C>::increment() noexcept
	{ ++this->index_; }

	template <std::size_t S, std::size_t C>
	void assembler_iterator<S, C>::decrement() noexcept
	{ --this->index_; }
}
