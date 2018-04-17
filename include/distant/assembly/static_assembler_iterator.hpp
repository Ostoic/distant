// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <boost/iterator/iterator_facade.hpp>

#include <distant/assembly/static_instruction.hpp>

namespace distant::assembly
{
	template <std::size_t AssemblerSize, std::size_t InstrCount>
	class static_assembler_iterator :
		public boost::iterator_facade<
			static_assembler_iterator<AssemblerSize, InstrCount>,
			static_instruction<AssemblerSize, InstrCount>,
			std::random_access_iterator_tag,
			static_instruction<AssemblerSize, InstrCount>
		>
	{
	public: // {ctor}
		constexpr static_assembler_iterator() noexcept;

		explicit constexpr static_assembler_iterator(
			const static_assembler<AssemblerSize, InstrCount>& assembler,
			index_t instruction_ptrs_index
		) noexcept;

	private:
		friend class boost::iterator_core_access;

		class enabler{};

		template <std::size_t S, std::size_t C>
		friend class static_assembler_iterator;

		template <std::size_t S, std::size_t C>
		constexpr bool equal(const static_assembler_iterator<S, C>& other) const noexcept;

		void increment() noexcept;

		void decrement() noexcept;

		constexpr static_instruction<AssemblerSize, InstrCount> 
			dereference() const noexcept;

	private:
		index_t instruction_ptrs_index_;
		const static_assembler<AssemblerSize, InstrCount>* assembler_;
	};
}

// Implementation:
#include <distant/assembly/impl/static_assembler_iterator.hxx>
