// distant dev.cpp : Defines the entry point for the console application.

#include "stdafx.h"

#define __cpp_lib_experimental_filesystem

#include <distant.hpp>
#include <distant\memory\ops.hpp>
#include <distant\memory\assembler.hpp>

int main()
{
	namespace ops = distant::memory::ops;
	using distant::memory::x86_register;

	constexpr distant::address entry_point = 0x400000;
	constexpr auto assembler =
		ops::pushad() +
		ops::push(2) +
		ops::pop(x86_register::eax) +
		ops::mov(x86_register::edx, ops::dword_ptr(x86_register::eax)) +
		ops::mov(x86_register::ecx, 2) +
		ops::popad() +
		ops::nop();

	constexpr auto array = meta::make_array(1, 2, 3, 4, 5);

	for (const auto& instr : assembler)
		std::cout << instr << '\n';
	std::cout << '\n';

	return 0;
}

