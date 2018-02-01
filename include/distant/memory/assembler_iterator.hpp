#pragma once

#include <boost\iterator\iterator_facade.hpp>

#include <distant\memory\instruction.hpp>

//#include <distant\memory\assembler.hpp>

namespace distant::memory
{
	template <std::size_t, std::size_t>
	class assembler;

	template <std::size_t AssemblerSize, std::size_t AssemblerCount>
	class assembler_iterator : 
		public boost::iterator_facade<
			assembler_iterator<AssemblerSize, AssemblerCount>,
			instruction<AssemblerSize, AssemblerCount>,
			std::random_access_iterator_tag,
			instruction<AssemblerSize, AssemblerCount>
		>
	{
	public: // {ctor}
		constexpr assembler_iterator() noexcept;

		explicit constexpr assembler_iterator(const assembler<AssemblerSize, AssemblerCount>& assembler, std::size_t index) noexcept;

	private:
		friend class boost::iterator_core_access;

		template <std::size_t S, std::size_t C>
		friend class assembler_iterator;

		template <std::size_t S, std::size_t C>
		constexpr bool equal(const assembler_iterator<S, C>& other) const noexcept;

		void increment() noexcept;

		void decrement() noexcept;

		constexpr instruction<AssemblerSize, AssemblerCount> dereference() const noexcept;

	private:
		std::size_t index_;
		const assembler<AssemblerSize, AssemblerCount>* assembler_;
	};
}

// Implementation:
#include <distant\memory\impl\assembler_iterator.hxx>