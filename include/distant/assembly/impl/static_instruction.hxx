// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once
#include "../static_instruction.hpp"

#include <distant/utility/streams.hpp>
#include <distant/assembly/ops.hpp>

#define MAKE_NAME_OPCODE_REG(op, reg) std::make_pair(opcode::##op##_##reg##, #op " " #reg)

#define MAKE_NAME_OPCODE_ALL(op) MAKE_NAME_OPCODE_REG(op, eax),\
							   MAKE_NAME_OPCODE_REG(op, ecx),\
							   MAKE_NAME_OPCODE_REG(op, edx),\
							   MAKE_NAME_OPCODE_REG(op, ebx),\
							   MAKE_NAME_OPCODE_REG(op, ebp),\
							   MAKE_NAME_OPCODE_REG(op, esi),\
							   MAKE_NAME_OPCODE_REG(op, edi)

namespace distant::assembly
{	
//class instruction:
//{ctor}:
	template <std::size_t S, std::size_t C>
	constexpr static_instruction<S, C>::static_instruction(const static_assembler<S, C>& assembler, const index_t instruction_ptr_index) noexcept
		: assembler_(assembler)
		, instruction_ptr_index_(instruction_ptr_index)
	{}

//interface:
	template <std::size_t S, std::size_t C>
	constexpr byte static_instruction<S, C>::operator[](const index_t index) const noexcept
	{
		const auto bytes_index = assembler_.instruction_ptrs_[instruction_ptr_index_];
		return assembler_.bytes[bytes_index + index];
	}

	template <std::size_t S, std::size_t C>
	byte& static_instruction<S, C>::operator[](const index_t index) noexcept
	{
		const auto bytes_index = assembler_.instruction_ptrs_[instruction_ptr_index_];
		return assembler_.bytes[bytes_index + index];
	}

	template <std::size_t S, std::size_t C>
	constexpr std::size_t static_instruction<S, C>::size() const noexcept
	{
		if (instruction_ptr_index_ + 1 == assembler_.instruction_ptrs_.size())
			return S - assembler_.instruction_ptrs_[instruction_ptr_index_];

		return assembler_.instruction_ptrs_[instruction_ptr_index_ + 1] - assembler_.instruction_ptrs_[instruction_ptr_index_];
	}

	namespace detail::string_maps
	{
		constexpr auto op_names = utility::meta::make_map(
			std::make_pair(opcode::call, "call"),
			MAKE_NAME_OPCODE_ALL(call),

			std::make_pair(opcode::jmp, "jmp"),
			MAKE_NAME_OPCODE_ALL(jmp),

			// Move opcodes
			MAKE_NAME_OPCODE_ALL(mov),
			std::make_pair(opcode::mov_eax_ptr, "mov eax, dword ptr:"),
			std::make_pair(opcode::mov_ebp_ptr, "mov ebp, dword ptr:"),
			std::make_pair(opcode::mov_ebx_ptr, "mov ebx, dword ptr:"),
			std::make_pair(opcode::mov_ecx_ptr, "mov ecx, dword ptr:"),
			std::make_pair(opcode::mov_edi_ptr, "mov edi, dword ptr:"),
			std::make_pair(opcode::mov_edx_ptr, "mov edx, dword ptr:"),
			std::make_pair(opcode::mov_esi_ptr, "mov esi, dword ptr:"),
			std::make_pair(opcode::mov_esp_ptr, "mov esp, dword ptr:"),

			// No operation
			std::make_pair(opcode::nop, "nop"),

			// Push opcodes
			std::make_pair(opcode::push, "push"),
			MAKE_NAME_OPCODE_ALL(push),
			std::make_pair(opcode::pushad, "pushad"),
			std::make_pair(opcode::pushfd, "pushfd"),

			// Pop opcodes
			MAKE_NAME_OPCODE_ALL(pop),
			std::make_pair(opcode::popad, "popad"),
			std::make_pair(opcode::popfd, "popfd")
			// TODO: Do the rest
		);
	}


	template <typename CharT, typename TraitsT, std::size_t S, std::size_t C>
	std::basic_ostream<CharT, TraitsT>& operator<<(std::basic_ostream<CharT, TraitsT>& stream, const static_instruction<S, C>& instr)
	{
		using char_t = typename std::basic_ostream<CharT, TraitsT>::char_type;
		static_assert(S >= 2, "[operator<<(instruction)] Invalid instruction length");

		using detail::string_maps::op_names;

		const auto write_byte = [&stream](const auto&& byte)
		{
			stream 
				<< "0x" << std::setw(2) << std::setfill(char_t{ '0' })
				<< static_cast<unsigned int>(byte) << ' ';
		};

		const auto old_flags = stream.flags();

		index_t i = 0;

		// Combine the first two bytes of the instruction to get the opcode.
		const auto op = static_cast<opcode>(
			distant::make_word(instr[0], instr[1])
		);

		// Test if the opcode is a valid entry among the opcode->name map
		if (op_names.count(op) != 0)
		{
			// Write the name of the opcode.
			if (!utility::is_hex(stream))
			{
				stream << op_names[op] << ' ';
				i = 2;
			}
			else if (instr[1] == 0)
			{
				write_byte(instr[0]);
				i = 2;
			}

			// Write the address as a distant::address to the stream. 
			const auto write_dword = [&i, &instr](auto& stream)
			{
				stream << distant::address(distant::make_dword(instr[i], instr[i + 1], instr[i + 2], instr[i + 3]));
				i += 4;
			};

			// If the opcode accepts a dword, then the next sizeof(distant::dword) bytes constitute a dword.
			if (ops::parameter_is_dword(op) && i + 3 < instr.size())
			{
				if (ops::is_deref_instruction(op))
				{
					stream << "[";
					write_dword(stream);
					stream << "]";
				}
				else
				{
					write_dword(stream);
					stream << ' ';
				}
			}
		}

		// Numbers will be written in hexadecimal.
		stream << std::hex;

		// Write the rest of the bytes to the stream.
		for (; i < instr.size(); ++i)
			write_byte(instr[i]);

		stream.flags(old_flags);
		return stream;
	}

}
