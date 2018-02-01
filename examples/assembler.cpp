#define __cpp_lib_experimental_filesystem
#define BOOST_USE_WINDOWS_H
#define NOMINMAX

#include <distant.hpp>
#include <distant\detail\to_string.hpp>
#include <distant\memory\ops.hpp>
#include <distant\memory\assembler.hpp>


int main()
{
	namespace ops = distant::memory::ops;
	constexpr distant::address entry = 0x40000;

	constexpr auto assembly =
		ops::push(2)	 +
		ops::call(entry) +
		ops::jmp(entry)  +
		ops::pushad()	 +
		ops::nop()		 +
		ops::nop();

	for (const auto& instr : assembly)
		std::cout << instr << '\n';

	std::cout << '\n';

    return 0;
}