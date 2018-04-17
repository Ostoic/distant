// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <utility> // std::size_t

namespace distant::assembly
{
	template <std::size_t, std::size_t>
	class static_assembler;

	template <std::size_t ByteCount, std::size_t InstructionCount>
	class static_instruction
	{
	public:
		constexpr static_instruction(const static_assembler<ByteCount, InstructionCount>& assembler, index_t instruction_ptr_index) noexcept;

		constexpr byte  operator[](std::size_t index) const noexcept;
				  byte& operator[](std::size_t index) noexcept;

		constexpr std::size_t size() const noexcept;

	private:
		const static_assembler<ByteCount, InstructionCount>& assembler_;
		index_t instruction_ptr_index_;
	};

	template <typename CharT, typename TraitsT, std::size_t ByteCount, std::size_t InstructionCount>
	std::basic_ostream<CharT, TraitsT>& operator<<(std::basic_ostream<CharT, TraitsT>& stream, const static_instruction<ByteCount, InstructionCount>& instr);
}

// Implementation:
#include "impl/static_instruction.hxx"
