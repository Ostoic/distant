#pragma once

#include <distant\types.hpp>
#include <distant\utility\constexpr_algorithms.hpp>

#include <distant\memory\instruction.hpp>
#include <distant\memory\assembler_iterator.hpp>

namespace distant::memory
{
	constexpr std::size_t address_length = sizeof(distant::address);

	template <std::size_t Size = 0, std::size_t InstrCount = 0>
	class assembler
	{
	public:
		using iterator = assembler_iterator<Size, InstrCount>;
		using const_iterator = iterator;

	public: // {ctor}
		constexpr assembler() = default;
		constexpr assembler(const assembler& other) noexcept = default;

	public: // interface
		constexpr iterator begin() const;
		constexpr iterator end() const;

	public: // operators
		template <typename... Bytes, 
			typename = std::enable_if_t<
				std::is_convertible_v<std::common_type_t<Bytes...>, distant::byte>
			>
		>
		friend constexpr auto make_instruction(opcode op, Bytes&&... bytes) noexcept;

		friend constexpr auto make_instruction(opcode op) noexcept;

		// TODO: Extend this to operator that can take a second argument with an arbitrary number of instructions.
		// ***** To do this, we need to transform the instruction_ptrs indices + InstrCount.
		template <std::size_t First, std::size_t FirstCount, std::size_t Second>
		friend constexpr assembler<First + Second, FirstCount + 1> 
		operator+(const assembler<First, FirstCount>& first, const assembler<Second, 1>& second) noexcept;

	private:
		using index = std::size_t;
		using instruction_length = std::size_t;

		constexpr assembler(
			const std::array<distant::byte, Size>& bytes,
			const std::array<std::pair<index, instruction_length>, InstrCount>& instruction_ptrs) noexcept;
	private:
		friend class instruction<Size, InstrCount>;
		friend class assembler_iterator<Size, InstrCount>;

		std::array<distant::byte, Size> bytes_;

		// instruction_ptrs_ holds indices to where each instruction in bytes_ is located,
		// along with the length of the instruction.
		std::array<std::pair<index, instruction_length>, InstrCount> instruction_ptrs_;
	};

	template <typename... Bytes, 
		typename = std::enable_if_t<
			std::is_convertible_v<std::common_type_t<Bytes...>, distant::byte> 
		>
	>
	constexpr auto make_instruction(opcode op, Bytes&&... bytes) noexcept;

	constexpr auto make_instruction(opcode op) noexcept;

	//constexpr auto make_instruction(opcode op) noexcept;
}

// Implementation:
#include <distant\memory\impl\assembler.hxx>