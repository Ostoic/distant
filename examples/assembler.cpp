// distant dev.cpp : Defines the entry point for the console application.

#define __cpp_lib_experimental_filesystem

#include <distant.hpp>
#include <distant/assembly/ops.hpp>
#include <distant/assembly/static_assembler.hpp>

int main()
{
	namespace ops = distant::assembly::ops;
	using distant::assembly::x86_register;

	constexpr distant::address entry_point = 0x400000;
	constexpr auto assembler =
		ops::pushad(); 

	/*+
		ops::push(2) +
		ops::pop(x86_register::eax) +
		ops::mov(x86_register::edx, ops::dword_ptr(x86_register::eax)) +
		ops::mov(x86_register::ecx, 2) +
		ops::popad() +
		ops::nop();*/

	//for (const auto& instr : assembler)
		//std::cout << instr << '\n';
	std::cout << '\n';
		
	return 0;
}

