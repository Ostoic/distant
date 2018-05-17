// distant dev.cpp : Defines the entry point for the console application.

#define __cpp_lib_experimental_filesystem

#include <distant.hpp>
#include <distant/assembly/ops.hpp>
#include <distant/assembly/static_assembler.hpp>

void test()
{
	int x;
}

int main()
{
	namespace ops = distant::assembly::ops;
	using distant::assembly::x86_register;
	using ops::dword_ptr;
	using distant::assembly::opcode;

	constexpr distant::address entry_point = 0x400000;

	// use user-defined literals for assembly instructions?
	// "mov esp, esp"_r ?
	constexpr auto assembler =
		ops::mov(x86_register::esp, x86_register::esp) +
		ops::push(x86_register::ebp) +
		ops::mov(x86_register::edx, distant::address()) +
		ops::call(x86_register::edx) +
		//ops::sub(x86_register::esp, 4) +
		ops::pop(x86_register::ebp);
		//ops::

	distant::word a = 0xA1B1;

	std::cout << "Byte = " << std::hex << static_cast<int>(distant::get_byte<1>(a)) << '\n';

	distant::memory::write<opcode>(distant::current_process(), test, opcode::mov_eax_ptr_ebp);
	std::cout << test << '\n';

	for (const auto instr : assembler)
		std::cout << instr << '\n';

	//for (const auto instruction : assembler)
		//std::cout << "Instruction size = " << instruction.size() << '\n';

	//std::cout << '\n';
	std::cin.ignore();
	return 0;
}

