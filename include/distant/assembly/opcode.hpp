// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/types.hpp>
#include <distant/memory/virtual_memory.hpp>

namespace distant::assembly
{
	/// @brief opcode enum containing x86 assembly opcode values.
	/// @remark enum bytes are in big-endian.
	enum class opcode : word
	{
		jmp_eax = 0xFFE0,
		jmp_ecx,
		jmp_edx,
		jmp_ebx,
		jmp_esp,
		jmp_ebp,
		jmp_esi,
		jmp_edi,
		jmp = 0xE8,

		call_eax = 0xFFD0,
		call_ecx,
		call_edx,
		call_ebx,
		call_esp,
		call_ebp,
		call_esi,
		call_edi,
		call = 0xE9,

		// add register, byte
		add_eax_byte = 0x83C0,
		add_ecx_byte,
		add_edx_byte,
		add_ebx_byte,
		add_esp_byte,
		add_ebp_byte,
		add_esi_byte,
		add_edi_byte,

		// sub register, byte
		sub_eax_byte = 0x83E8,
		sub_ecx_byte,
		sub_edx_byte,
		sub_ebx_byte,
		sub_esp_byte,
		sub_ebp_byte,
		sub_esi_byte,
		sub_edi_byte,

		// add register, dword
		add_eax_dword = 0x05,
		add_ecx_dword = 0x81C1,
		add_edx_dword,
		add_ebx_dword,
		add_esp_dword,
		add_ebp_dword,
		add_esi_dword,
		add_edi_dword,

		// sub register, dword
		sub_eax_dword = 0x2D,
		sub_ecx_dword = 0x81E9,
		sub_edx_dword,
		sub_ebx_dword,
		sub_esp_dword,
		sub_ebp_dword,
		sub_esi_dword,
		sub_edi_dword,

		// mov register, dword
		mov_eax = 0xB8,
		mov_ecx,
		mov_edx,
		mov_ebx,
		mov_esp,
		mov_ebp,
		mov_esi,
		mov_edi,

		// mov register, register
		mov_eax_eax = 0x8BC0,
		mov_eax_ecx,
		mov_eax_edx,
		mov_eax_ebx,
		mov_eax_esp,
		mov_eax_ebp,
		mov_eax_esi,
		mov_eax_edi,

		mov_ecx_eax,
		mov_ecx_ecx,
		mov_ecx_edx,
		mov_ecx_ebx,
		mov_ecx_esp,
		mov_ecx_ebp,
		mov_ecx_esi,
		mov_ecx_edi,

		mov_edx_eax,
		mov_edx_ecx,
		mov_edx_edx,
		mov_edx_ebx,
		mov_edx_esp,
		mov_edx_ebp,
		mov_edx_esi,
		mov_edx_edi,

		mov_ebx_eax,
		mov_ebx_ecx,
		mov_ebx_edx,
		mov_ebx_ebx,
		mov_ebx_esp,
		mov_ebx_ebp,
		mov_ebx_esi,
		mov_ebx_edi,

		mov_esp_eax,
		mov_esp_ecx,
		mov_esp_edx,
		mov_esp_ebx,
		mov_esp_esp,
		mov_esp_ebp,
		mov_esp_esi,
		mov_esp_edi,

		mov_ebp_eax,
		mov_ebp_ecx,
		mov_ebp_edx,
		mov_ebp_ebx,
		mov_ebp_esp,
		mov_ebp_ebp,
		mov_ebp_esi,
		mov_ebp_edi,

		mov_esi_eax,
		mov_esi_ecx,
		mov_esi_edx,
		mov_esi_ebx,
		mov_esi_esp,
		mov_esi_ebp,
		mov_esi_esi,
		mov_esi_edi,

		mov_edi_eax,
		mov_edi_ecx,
		mov_edi_edx,
		mov_edi_ebx,
		mov_edi_esp,
		mov_edi_ebp,
		mov_edi_esi,
		mov_edi_edi,

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

	}; // enum opcode

	constexpr std::size_t opcode_length(const opcode code) noexcept
	{
		std::size_t length = 0;

		// If the second byte is nonzero, the whole 2 bytes form an opcode
		if (get_byte<1>(static_cast<word>(code)) != 0)
			length = 2;

		// Otherwise we have a 1 byte opcode
		else if (get_byte<0>(static_cast<word>(code)) != 0)
			length = 1;

		return length;
	}

} // namespace distant::assembly

namespace distant::memory
{
	template <>
	struct operations_traits<assembly::opcode>
	{
		template <typename AddressT>
		static void write(const process<vm_w_op>& proc, const address<AddressT> address, const assembly::opcode opcode)
		{
			using under_t = std::underlying_type_t<assembly::opcode>;

			// Write the bytes of the given opcode in reverse due to endianness
			operations_traits<under_t>::write(proc, address, reverse_bytes(static_cast<under_t>(opcode)));
		}

		template <typename AddressT>
		static assembly::opcode read(const process<vm_read>& proc, const address<AddressT> address)
		{
			using under_t = std::underlying_type_t<assembly::opcode>;
			return static_cast<assembly::opcode>(reverse_bytes(operations_traits<under_t>::read(proc, address)));
		}

	}; // struct operations_traits<assembly_opcode>

} // namespace distant::memory
