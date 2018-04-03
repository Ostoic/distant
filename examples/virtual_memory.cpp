// distance dev.cpp : Defines the entry point for the console application.

#include <iostream>
#include <distant/virtual_memory.hpp>

int main()
{
	const auto current = distant::current_process<>();

	const auto ptr = distant::virtual_malloc<int>(current);
	*ptr = 2;

	std::cout << "t = " << ptr << '\n';
	std::cout << "*t = " << *ptr << '\n';
	distant::virtual_free(current, ptr);
}
