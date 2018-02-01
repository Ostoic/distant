#pragma once

#include <distant\types.hpp>

namespace distant::memory
{
	enum class opcode : unsigned short
	{
		jmp  = 0xE8,

		call_eax = 0xFFD0,
		call_ecx,
		call_edx,
		call_ebx,
		call_esp,
		call_ebp,
		call_esi,
		call_edi,
		call_32bit = 0xE9,

		// Mov register, parameter
		mov_eax = 0xB8,
		mov_ecx,
		mov_edx,
		mov_ebx,
		mov_esp,
		mov_ebp,
		mov_esi,
		mov_edi,

		// Push opcodes
		push_eax = 0x50,
		push_ecx,
		push_edx,
		push_ebx,
		push_esp,
		push_ebp,
		push_esi,
		push_edi ,
		push = 0x6A,
		push_32bit = 0x68,
		pushad = 0x60,
		pushfd = 0x9C,

		// Pop opcodes
		pop_eax = 0x58,
		pop_ecx,
		pop_edx,
		pop_ebx,
		pop_esp,
		pop_ebp,
		pop_esi,
		pop_edi,
		popad = 0x6B,
		popfd = 0x9D,

		nop = 0x90,
	};
}
