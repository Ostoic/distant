#pragma once

#include <distant/assembly/opcode.hpp>

namespace distant::assembly
{
	namespace literals
	{
		/**
		 * todo: constexpr recursive-descent parser for asm string literals
		 * intel x86 assembly context-free grammar
		 * todo: more opcodes, more instruction formats
		 * opcode ::= "mov"|"push"|"pop"|"sub"|"add"|"call"|"jmp"|"nop";
		 * 
		 * register ::= "eax"|"ecx"|"edx"|"ebx"|"esp"|"ebp"|"esi"|"edi
		 * 
		 * instruction ::= opcode
		 *				| opcode register, register
		 *				| opcode register, number
		 *				| opcode register, ptr
		 *				| opcode register
		 *				;
		 */
		constexpr opcode parse_op(const char* string, const std::size_t length)
		{
			return opcode::add_eax_byte;
		}

		// Todo: Return static assembler somehow
		constexpr opcode operator""_assm(const char* string, const std::size_t length)
		{
			return parse_op(string, length);
		}
	}
}
