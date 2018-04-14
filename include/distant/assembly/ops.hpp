// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/assembly/static_assembler.hpp>
#include <distant/memory/address.hpp>
#include <distant/assembly/dword_ptr.hpp>
#include <distant/assembly/x86_register.hpp>

namespace distant::assembly::ops
{
	constexpr dword_ptr_t<distant::address> dword_ptr(distant::address) noexcept;
	constexpr dword_ptr_t<x86_register> dword_ptr(x86_register) noexcept;

	constexpr static_assembler<sizeof(opcode) + sizeof(distant::address), 1>
	call(distant::address) noexcept;

	/**********************************************************************************************/ /**
	 * @fn constexpr assembler<sizeof(opcode), 1> call(x86_register);
	 *
	 * @brief Generate a call assembly instruction that calls the address contained in the given resister.
	 *
	 * @throws std::logic_error when provided with the eip register.
	 * @param r the x86_register call.
	 * @return An assembler with the given call instruction.
	 **************************************************************************************************/
	constexpr static_assembler<sizeof(opcode), 1>
	call(x86_register r);

	constexpr static_assembler<sizeof(opcode) + sizeof(distant::address), 1>
	jmp(distant::address a) noexcept;

	/**********************************************************************************************/ /**
	 * @fn constexpr assembler<sizeof(opcode), 1> jmp(x86_register r) noexcept;
	 *
	 * @brief Generate a jmp assembly instruction that jumps to the address contained in 
	 * 		  the given register.
	 *
	 * @param r The x86_register containing the address.
	 *
	 * @return An assembler with the given jmp instruction.
	 **************************************************************************************************/
	constexpr static_assembler<sizeof(opcode), 1>
	jmp(x86_register r) noexcept;

	constexpr static_assembler<sizeof(opcode) + sizeof(distant::address), 1>
	mov(x86_register r, distant::address address) noexcept;

	constexpr static_assembler<sizeof(opcode) + sizeof(dword), 1>
	mov(x86_register r, dword param) noexcept;

	constexpr static_assembler<sizeof(opcode) + sizeof(dword), 1>
	mov(x86_register r, dword_ptr_t<distant::address> address);

	constexpr static_assembler<sizeof(opcode), 1>
	mov(x86_register r, dword_ptr_t<x86_register> rptr);

	constexpr static_assembler<sizeof(opcode) + sizeof(byte), 1>
	push(byte b) noexcept;

	// TODO: Should dispatch to corresponding opcode based on the given register.
	// Push from the given register onto the stack.
	constexpr static_assembler<sizeof(opcode), 1>
	push(x86_register r) noexcept;

	constexpr static_assembler<sizeof(opcode), 1>
	pushad() noexcept;

	// Pop off the stack into the given register
	constexpr static_assembler<sizeof(opcode), 1>
	pop(x86_register r) noexcept;

	constexpr static_assembler<sizeof(opcode), 1>
	popad() noexcept;

	constexpr static_assembler<sizeof(opcode), 1>
	nop() noexcept;

	constexpr bool is_deref_instruction(opcode) noexcept;

	constexpr bool accepts_address(opcode) noexcept;

	constexpr bool parameter_is_dword(opcode op) noexcept;
}

#include "impl/ops.hxx"
