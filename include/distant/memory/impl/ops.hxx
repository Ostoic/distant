#pragma once
#include <distant\memory\ops.hpp>

#include <distant\memory\opcode.hpp>

#include <distant\utility\map.hpp>

namespace distant::memory::ops
{
//interface: asm instructions
	constexpr assembler<1 + address_length, 1> call(distant::address address) noexcept
	{
		using distant::memory::get;

		return make_instruction(opcode::call_32bit, get<0>(address), get<1>(address), get<2>(address), get<3>(address));
	}

	/*constexpr assembler<2, 1> call(const x86_register reg) noexcept
	{
		constexpr auto map = meta::make_map(
			std::make_pair(x86_register::eax, opcode::call_eax),
			std::make_pair(x86_register::ebx, opcode::call_ebx),
			std::make_pair(x86_register::ecx, opcode::call_ecx),
			std::make_pair(x86_register::edx, opcode::call_edx),
			std::make_pair(x86_register::ebp, opcode::call_ebp),
			std::make_pair(x86_register::esp, opcode::call_esp),
			std::make_pair(x86_register::edi, opcode::call_edi),
			std::make_pair(x86_register::esi, opcode::call_esi)
		);

		return make_instruction(map[reg]);
	}*/


	constexpr assembler<1 + address_length, 1> jmp(distant::address address) noexcept
	{
		using distant::memory::get;

		return make_instruction(opcode::jmp, get<0>(address), get<1>(address), get<2>(address), get<3>(address));
	}

	constexpr assembler<2, 1> push(distant::byte argument) noexcept
	{
		return make_instruction(opcode::push, argument);
	}

	constexpr assembler<1, 1> pushad() noexcept
	{
		return make_instruction(opcode::pushad);
	}

	constexpr assembler<1, 1> popad() noexcept
	{
		return make_instruction(opcode::popad);
	}

	constexpr assembler<1, 1> nop() noexcept
	{
		return make_instruction(opcode::nop);
	}

} // namespace distant::memory::instructions
