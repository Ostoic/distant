#pragma once

#include <distant\types.hpp>

namespace distant::memory
{
	enum class opcode : distant::word
	{
		jmp_eax = 0xFFE0,
		jmp_ecx,
		jmp_edx,
		jmp_ebx,
		jmp_esp,
		jmp_ebp,
		jmp_esi,
		jmp_edi,
		jmp_32bit = 0xE8,

		call_eax = 0xFFD0,
		call_ecx,
		call_edx,
		call_ebx,
		call_esp,
		call_ebp,
		call_esi,
		call_edi,
		call_32bit = 0xE9,

		// mov register, dword
		mov_eax = 0xB8,
		mov_ecx,
		mov_edx,
		mov_ebx,
		mov_esp,
		mov_ebp,
		mov_esi,
		mov_edi,

		// mov register, dword ptr:[dword]
		mov_eax_ptr,
		mov_ecx_ptr,
		mov_edx_ptr,
		mov_ebx_ptr,
		mov_esp_ptr,
		mov_ebp_ptr,
		mov_esi_ptr,
		mov_edi_ptr,

		// mov eax, dword ptr [register]
		mov_eax_ptr_eax = 0x8B00,
		mov_eax_ptr_ecx,
		mov_eax_ptr_edx,
		mov_eax_ptr_ebx,
		mov_eax_ptr_esp,
		mov_eax_ptr_ebp,
		mov_eax_ptr_esi,
		mov_eax_ptr_edi,

		// mov ecx, dword ptr [register]
		mov_ecx_ptr_eax,
		mov_ecx_ptr_ecx,
		mov_ecx_ptr_edx,
		mov_ecx_ptr_ebx,
		mov_ecx_ptr_esp,
		mov_ecx_ptr_ebp,
		mov_ecx_ptr_esi,
		mov_ecx_ptr_edi,

		// mov edx, dword ptr [register]
		mov_edx_ptr_eax,
		mov_edx_ptr_ecx,
		mov_edx_ptr_edx,
		mov_edx_ptr_ebx,
		mov_edx_ptr_esp,
		mov_edx_ptr_ebp,
		mov_edx_ptr_esi,
		mov_edx_ptr_edi,

		// mov ebx, dword ptr [register]
		mov_ebx_ptr_eax,
		mov_ebx_ptr_ecx,
		mov_ebx_ptr_edx,
		mov_ebx_ptr_ebx,
		mov_ebx_ptr_esp,
		mov_ebx_ptr_ebp,
		mov_ebx_ptr_esi,
		mov_ebx_ptr_edi,

		// mov esp, dword ptr [register]
		mov_esp_ptr_eax,
		mov_esp_ptr_ecx,
		mov_esp_ptr_edx,
		mov_esp_ptr_ebx,
		mov_esp_ptr_esp,
		mov_esp_ptr_ebp,
		mov_esp_ptr_esi,
		mov_esp_ptr_edi,

		// mov ebp, dword ptr [register]
		mov_ebp_ptr_eax,
		mov_ebp_ptr_ecx,
		mov_ebp_ptr_edx,
		mov_ebp_ptr_ebx,
		mov_ebp_ptr_esp,
		mov_ebp_ptr_ebp,
		mov_ebp_ptr_esi,
		mov_ebp_ptr_edi,

		// mov esi, dword ptr [register]
		mov_esi_ptr_eax,
		mov_esi_ptr_ecx,
		mov_esi_ptr_edx,
		mov_esi_ptr_ebx,
		mov_esi_ptr_esp,
		mov_esi_ptr_ebp,
		mov_esi_ptr_esi,
		mov_esi_ptr_edi,

		// mov edi, dword ptr [register]
		mov_edi_ptr_eax,
		mov_edi_ptr_ecx,
		mov_edi_ptr_edx,
		mov_edi_ptr_ebx,
		mov_edi_ptr_esp,
		mov_edi_ptr_ebp,
		mov_edi_ptr_esi,
		mov_edi_ptr_edi,

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

	constexpr std::size_t opcode_size = sizeof(opcode);
}
