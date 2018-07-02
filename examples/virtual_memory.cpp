// distance dev.cpp : Defines the entry point for the console application.

#include <iostream>
#include <distant/virtual_memory.hpp>

int main()
{
	auto current = distant::current_process();

	const auto ptr = distant::virtual_malloc<int>(current);
	*ptr = 2;

	distant::virtual_protect<distant::page_protection::readonly>(current, ptr.get(), sizeof(decltype(ptr)::element_type));
	std::cout << "t = " << ptr << '\n';
	std::cout << "*t = " << *ptr << '\n';
	std::cout << "read = " << distant::memory::read<int>(current, ptr.get()) << '\n';

	distant::virtual_protect<distant::page_protection::readwrite>(current, ptr.get(), sizeof(decltype(ptr)::element_type));
	*ptr = 3;
	std::cout << "*t = " << *ptr << '\n';
	distant::virtual_free(current, ptr);
}
