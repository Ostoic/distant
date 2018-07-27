// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once
#include <distant/assembly/ops.hpp>

#include <distant/assembly/opcode.hpp>
#include <distant/utility/meta/map.hpp>

#define MAKE_REGISTER_PAIR(op, reg) std::make_pair(x86_register::##reg##, opcode::##op##_##reg##)

#define MAKE_REGISTER_ALL(op) MAKE_REGISTER_PAIR(op, eax),\
							   MAKE_REGISTER_PAIR(op, ecx),\
							   MAKE_REGISTER_PAIR(op, edx),\
							   MAKE_REGISTER_PAIR(op, ebx),\
							   MAKE_REGISTER_PAIR(op, esp),\
							   MAKE_REGISTER_PAIR(op, ebp),\
							   MAKE_REGISTER_PAIR(op, esi),\
							   MAKE_REGISTER_PAIR(op, edi)

#define MAKE_REGISTER_PTR_PAIR(reg1, reg2) std::make_pair(std::make_pair(x86_register::reg1, x86_register::reg2), opcode::mov_##reg1##_ptr_##reg2)

#define MAKE_REGISTER_PTR_ALL(reg) MAKE_REGISTER_PTR_PAIR(reg, eax),\
							   MAKE_REGISTER_PTR_PAIR(reg, ecx),\
							   MAKE_REGISTER_PTR_PAIR(reg, edx),\
							   MAKE_REGISTER_PTR_PAIR(reg, ebx),\
							   MAKE_REGISTER_PTR_PAIR(reg, esp),\
							   MAKE_REGISTER_PTR_PAIR(reg, ebp),\
							   MAKE_REGISTER_PTR_PAIR(reg, esi),\
							   MAKE_REGISTER_PTR_PAIR(reg, edi)

#define MAKE_REGISTER_REGISTER_PAIR(reg1, reg2) std::make_pair(std::make_pair(x86_register::reg1, x86_register::reg2), opcode::mov_##reg1##_##reg2)

#define MAKE_REGISTER_REGISTER_ALL(reg) MAKE_REGISTER_REGISTER_PAIR(reg, eax),\
							   MAKE_REGISTER_REGISTER_PAIR(reg, ecx),\
							   MAKE_REGISTER_REGISTER_PAIR(reg, edx),\
							   MAKE_REGISTER_REGISTER_PAIR(reg, ebx),\
							   MAKE_REGISTER_REGISTER_PAIR(reg, esp),\
							   MAKE_REGISTER_REGISTER_PAIR(reg, ebp),\
							   MAKE_REGISTER_REGISTER_PAIR(reg, esi),\
							   MAKE_REGISTER_REGISTER_PAIR(reg, edi)

namespace distant::assembly::ops
{
//interface: asm instructions

	constexpr static_assembler<sizeof(opcode) + sizeof(distant::address), 1>
		call(const address address) noexcept
	{
		using memory::get_byte;
		return make_instruction(opcode::call, get_byte<0>(address), get_byte<1>(address), get_byte<2>(address), get_byte<3>(address));
	}

	constexpr static_assembler<sizeof(opcode), 1>
		call(const x86_register r)
	{
		using utility::meta::make_map;
		constexpr auto map = make_map(
			MAKE_REGISTER_ALL(call)
		);

		if (r == x86_register::eip)
			throw std::logic_error("[ops::call] Cannot call the eip register");

		return make_instruction(map[r]);
	}

	constexpr static_assembler<sizeof(opcode) + sizeof(distant::address), 1>
		jmp(const distant::address address) noexcept
	{
		using memory::get_byte;

		return make_instruction(opcode::jmp, get_byte<0>(address), get_byte<1>(address), get_byte<2>(address), get_byte<3>(address));
	}

	constexpr static_assembler<sizeof(opcode), 1>
		jmp(const x86_register r) noexcept
	{
		using utility::meta::make_map;
		constexpr auto map = make_map(
			MAKE_REGISTER_ALL(jmp)
		);

		return make_instruction(map[r]);
	}

	template <typename Scalar>
	constexpr auto sub(x86_register r, Scalar scalar) noexcept
	{
		using utility::meta::make_map;
		constexpr auto map = make_map(
			//std::make_pair(),
		);

		return make_instruction();
	}

	template <typename Scalar>
	constexpr auto add(x86_register r, Scalar scalar) noexcept;

	constexpr static_assembler<sizeof(opcode) + sizeof(distant::address), 1>
		mov(const x86_register r, distant::address a) noexcept
	{
		using memory::get_byte;
		constexpr auto map = utility::meta::make_map(
			MAKE_REGISTER_ALL(mov)
		);

		return make_instruction(map[r], get_byte<0>(a), get_byte<1>(a), get_byte<2>(a), get_byte<3>(a));
	}

	/*constexpr static_assembler<sizeof(opcode) + sizeof(distant::address), 1>
		mov(const x86_register r, const dword param) noexcept
	{
		using distant::get_byte;
		constexpr auto map = utility::meta::make_map(
			MAKE_REGISTER_ALL(mov)
		);

		return make_instruction(map[r], get_byte<0>(param), get_byte<1>(param), get_byte<2>(param), get_byte<3>(param));
	}*/

	//constexpr static_assembler<sizeof(opcode) + sizeof(distant::dword), 1>
	//	mov(const x86_register r, const dword_ptr_t<distant::address> add_ptr)
	//{
	//	using distant::get_byte;
	//	constexpr auto map = utility::meta::make_map(
	//		std::make_pair(x86_register::eax, opcode::mov_eax_ptr),
	//		std::make_pair(x86_register::ebx, opcode::mov_ebx_ptr),
	//		std::make_pair(x86_register::ecx, opcode::mov_ecx_ptr),
	//		std::make_pair(x86_register::edx, opcode::mov_edx_ptr),
	//		std::make_pair(x86_register::ebp, opcode::mov_ebp_ptr),
	//		std::make_pair(x86_register::esp, opcode::mov_esp_ptr),
	//		std::make_pair(x86_register::edi, opcode::mov_edi_ptr),
	//		std::make_pair(x86_register::esi, opcode::mov_esi_ptr)
	//	);

	//	const auto address = add_ptr.data;
	//	//return make_instruction(map[r], get_byte<0>(address), get_byte<1>(address), get_byte<2>(address), get_byte<3>(address));
	//}

	constexpr static_assembler<sizeof(opcode), 1>
		mov(const x86_register r, dword_ptr_t<x86_register> reg)
	{
		constexpr auto map = utility::meta::make_map(
			MAKE_REGISTER_PTR_ALL(eax),
			MAKE_REGISTER_PTR_ALL(ecx),
			MAKE_REGISTER_PTR_ALL(edx),
			MAKE_REGISTER_PTR_ALL(ebx),
			MAKE_REGISTER_PTR_ALL(esp),
			MAKE_REGISTER_PTR_ALL(ebp),
			MAKE_REGISTER_PTR_ALL(esi),
			MAKE_REGISTER_PTR_ALL(edi)
		);

		const auto pair = std::make_pair(r, reg.data);
		return make_instruction(map[pair]);
	}

	constexpr static_assembler<sizeof(opcode), 1>
		mov(x86_register to, x86_register from)
	{
		using distant::get_byte;
		constexpr auto map = utility::meta::make_map(
			MAKE_REGISTER_REGISTER_ALL(eax),
			MAKE_REGISTER_REGISTER_ALL(ecx),
			MAKE_REGISTER_REGISTER_ALL(edx),
			MAKE_REGISTER_REGISTER_ALL(ebx),
			MAKE_REGISTER_REGISTER_ALL(esp),
			MAKE_REGISTER_REGISTER_ALL(ebp),
			MAKE_REGISTER_REGISTER_ALL(esi),
			MAKE_REGISTER_REGISTER_ALL(edi)
		);

		return make_instruction(map[std::make_pair(to, from)]);
	}

	constexpr static_assembler<sizeof(opcode) + sizeof(byte), 1>
		push(const distant::byte argument) noexcept
	{
		return make_instruction(opcode::push, argument);
	}

	constexpr static_assembler<sizeof(opcode), 1>
		push(const x86_register r) noexcept
	{
		using utility::meta::make_map;
		constexpr auto map = make_map(
			std::make_pair(x86_register::eax, opcode::push_eax),
			std::make_pair(x86_register::ebx, opcode::push_ebx),
			std::make_pair(x86_register::ecx, opcode::push_ecx),
			std::make_pair(x86_register::edx, opcode::push_edx),
			std::make_pair(x86_register::ebp, opcode::push_ebp),
			std::make_pair(x86_register::esp, opcode::push_esp),
			std::make_pair(x86_register::edi, opcode::push_edi),
			std::make_pair(x86_register::esi, opcode::push_esi)
		);

		return make_instruction(map[r]);
	}

	constexpr static_assembler<sizeof(opcode), 1>
		pushad() noexcept
	{
		return make_instruction(opcode::pushad);
	}

	// Pop off the stack into the given register
	constexpr static_assembler<sizeof(opcode), 1>
		pop(const x86_register r) noexcept
	{
		constexpr auto map = utility::meta::make_map(
			std::make_pair(x86_register::eax, opcode::pop_eax),
			std::make_pair(x86_register::ebx, opcode::pop_ebx),
			std::make_pair(x86_register::ecx, opcode::pop_ecx),
			std::make_pair(x86_register::edx, opcode::pop_edx),
			std::make_pair(x86_register::ebp, opcode::pop_ebp),
			std::make_pair(x86_register::esp, opcode::pop_esp),
			std::make_pair(x86_register::edi, opcode::pop_edi),
			std::make_pair(x86_register::esi, opcode::pop_esi)
		);

		return make_instruction(map[r]);
	}

	constexpr static_assembler<sizeof(opcode), 1>
		popad() noexcept
	{
		return make_instruction(opcode::popad);
	}

	constexpr static_assembler<sizeof(opcode), 1>
		nop() noexcept
	{
		return make_instruction(opcode::nop);
	}

	constexpr bool is_deref_instruction(const opcode op) noexcept
	{
		return
			(opcode::mov_eax_ptr_eax <= op && op <= opcode::mov_esp_ptr_esp) ||
			(opcode::mov_eax_ptr <= op && op <= opcode::mov_edi_ptr);
	}

	constexpr bool accepts_address(const opcode op) noexcept
	{
		return is_deref_instruction(op) || (
			op == opcode::call ||
			op == opcode::jmp
		);
	}

	constexpr bool parameter_is_dword(const opcode op) noexcept
	{
		return accepts_address(op) || (
			op == opcode::mov_eax ||
			op == opcode::mov_ebp ||
			op == opcode::mov_ebx ||
			op == opcode::mov_ecx ||
			op == opcode::mov_edi ||
			op == opcode::mov_edx ||
			op == opcode::mov_esi ||
			op == opcode::mov_esp
		);
	}
} // namespace distant::memory::instructions
