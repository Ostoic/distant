// distance dev.cpp : Defines the entry point for the console application.

#include <iostream>
#include <distant.hpp>
#include <distant/memory/virtual_ptr.hpp>
#include <distant/memory/virtual_reference.hpp>

int main()
{
	const auto current = distant::current_process<>();

	int x = 3;

	distant::address address = &x;
	const auto y = distant::make_virtual_ptr<int>(current, &x);
	*y = 10;



	std::cout << *y << '\n';
}
