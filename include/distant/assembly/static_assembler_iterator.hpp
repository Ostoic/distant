#pragma once

#include <boost/iterator/iterator_facade.hpp>

#include <distant/assembly/static_instruction.hpp>

namespace distant::memory
{
	template <std::size_t, std::size_t>
	class static_assembler;

	template <std::size_t AssemblerSize, std::size_t AssemblerCount>
	class static_assembler_iterator :
		public boost::iterator_facade<
			static_assembler_iterator<AssemblerSize, AssemblerCount>,
			static_instruction<AssemblerSize, AssemblerCount>,
			std::random_access_iterator_tag,
			static_instruction<AssemblerSize, AssemblerCount>
		>
	
	{
	public: // {ctor}
		constexpr static_assembler_iterator() noexcept;

		explicit constexpr static_assembler_iterator(const static_assembler<AssemblerSize, AssemblerCount>& assembler,
		                                      std::size_t index) noexcept;

	private:
		friend class boost::iterator_core_access;

		template <std::size_t S, std::size_t C>
		friend class static_assembler_iterator;

		template <std::size_t S, std::size_t C>
		constexpr bool equal(const static_assembler_iterator<S, C>& other) const noexcept;

		void increment() noexcept;

		void decrement() noexcept;

		constexpr static_instruction<AssemblerSize, AssemblerCount> dereference() const noexcept;

	private:
		std::size_t index_;
		const static_assembler<AssemblerSize, AssemblerCount>* assembler_;
	};
}

// Implementation:
#include <distant/assembly/impl/static_assembler_iterator.hxx>
