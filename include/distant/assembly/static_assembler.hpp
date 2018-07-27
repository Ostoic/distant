// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/types.hpp>
#include <distant/utility/meta/array.hpp>

#include <distant/assembly/opcode.hpp>
#include <distant/assembly/static_instruction.hpp>
#include <distant/assembly/static_assembler_iterator.hpp>

namespace distant::assembly
{
	template <std::size_t ByteCount = 0, std::size_t InstructionCount = 0>
	class static_assembler
	{
	public:
		using const_iterator = static_assembler_iterator<ByteCount, InstructionCount>;
		using iterator = const_iterator;

	public: // {ctor}
		constexpr static_assembler() = default;

	public: // interface
		constexpr iterator begin() const noexcept;
		constexpr iterator end() const noexcept;

		static constexpr std::size_t instruction_count() noexcept { return InstructionCount; }
		static constexpr std::size_t byte_count() noexcept { return InstructionCount; }

	public:
	//private:
		constexpr static_assembler(
			std::array<byte, ByteCount> bytes,
			std::array<index_t, InstructionCount> instruction_ptrs) noexcept;

		template <std::size_t FirstByteCount, std::size_t FirstInstructionCount, std::size_t Second>
		constexpr static_assembler<FirstByteCount + Second, FirstInstructionCount + 1>
		friend operator+(
				const static_assembler<FirstByteCount, FirstInstructionCount>& first,
				const static_assembler<Second, 1>& second) noexcept;

		constexpr auto operator[](unsigned int index) const noexcept;

		std::array<byte, ByteCount> bytes;

	private:
		template <std::size_t S, std::size_t IC>
		friend class static_instruction;

		template <std::size_t S, std::size_t IC>
		friend class static_assembler_iterator;

		const std::array<index_t, InstructionCount> instruction_ptrs_;
	};

	template <typename... Bytes,
		typename = std::enable_if_t<
			std::is_convertible_v<std::common_type_t<Bytes...>, byte>
	    >
	>
	constexpr auto make_instruction(opcode op, Bytes&&... bytes) noexcept;

	constexpr auto make_instruction(opcode op) noexcept;

	// TODO: Extend this to operator that can take a second argument with an arbitrary number of instructions.
	// ***** To do this, we need to transform the instruction_ptrs indices + InstructionCount.

	/**********************************************************************************************/ /**
	 * @brief Access the nth instruction of the given assembler.
	 *
	 * @tparam N The index of the assembler to access.
	 * @tparam Size The size of the assembler.
	 * @tparam InstrCount The number of instructions that the assembler can hold.
	 * @param a An assembler of the given size and instruction count.
	 *
	 * @return An instruction of the corresponding size.
	 **************************************************************************************************/
	template <std::size_t N, std::size_t Size, std::size_t InstrCount>
	constexpr auto get_instruction(const static_assembler<Size, InstrCount>& a) noexcept;
}

// Implementation:
#include "impl/static_assembler.hxx"
