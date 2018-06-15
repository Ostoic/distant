#pragma once

#include <vector>

#include <distant/assembly/opcode.hpp>

namespace distant::assembly
{
	class assembler
	{
	public:
		void jmp();
		void call();

		void add();
		void sub();
		void mov();
		void push();
		void pop();
		void pushad();
		void pushfd();
		void popad();
		void popfd();

		void nop();
	private:

	};
}
