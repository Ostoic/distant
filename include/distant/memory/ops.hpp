#pragma once

#include <distant\memory\assembler.hpp>
#include <distant\memory\address.hpp>
#include <distant\memory\x86_register.hpp>

namespace distant::memory::ops
{
	constexpr assembler<1 + address_length, 1> call(distant::address) noexcept;
	constexpr assembler<2, 1>				   call(x86_register) noexcept;

	constexpr assembler<1 + address_length, 1> jmp(distant::address) noexcept;
	constexpr assembler<2, 1>				   jmp(x86_register) noexcept;

	constexpr assembler<3 + address_length, 1> mov(x86_register, distant::address) noexcept;
	constexpr assembler<2, 1>				   mov(x86_register, distant::byte) noexcept;

	constexpr assembler<2, 1> push(distant::byte) noexcept;

	// TODO: Should dispatch to corresponding opcode based on the given register.
	// Push from the given register onto the stack.
	constexpr assembler<2, 1> push(/*register*/) noexcept;
	constexpr assembler<1, 1> pushad() noexcept;

	// Pop off the stack into the given register
	constexpr assembler<2, 1> pop(/*register*/) noexcept;
	constexpr assembler<1, 1> popad() noexcept;
	constexpr assembler<1, 1> nop() noexcept;
}

#include <distant\memory\impl\ops.hxx>