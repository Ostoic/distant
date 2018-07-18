// distance dev.cpp : Defines the entry point for the console application.

#include <iostream>

#include <distant/virtual_memory.hpp>
#include <distant/process.hpp>

#define NOMINMAX

void* operator new(const std::size_t sz)
{
	std::cout << "[NEW] new operator called\n";
	return std::malloc(sz);
}

template <class Tuple>
void tuple_print(Tuple&& tuple)
{
	using namespace distant::utility;
	meta::tuple_for_each(std::forward<Tuple>(tuple), [](const auto& element)
	{
		std::cout << element << ", ";
	});

	std::cout << '\n';
}

struct S
{
	int a;
	int b;
	short c;
	//std::string d;
	double x;
	double y;
	int z;
};

std::ostream& operator<<(std::ostream& stream, const S& s)
{
	return (stream << s.a << ", "
		<< s.b << ", "
		<< s.c << ", "
		<< s.x << ", "
		<< s.y << ", "
		<< s.z);
}

struct test
{
	int a;
	char b[16];
	short c;
	double d;
};

std::ostream& operator<<(std::ostream& stream, const test& s)
{
	return (stream << s.a << ", "
		<< s.b << ", "
		<< s.c << ", "
		<< s.d);
}

int main()
{
	using namespace distant;
	using namespace utility;

	auto current = distant::current_process();

	S s = { 1, 2, 3, 6.1, 5.5, 0 };
	test t = { 4, "hello", -1, 5.5 };

	using T = const char[16];
	T x = { 1, 2 };
	//const char[16];

	std::cout << current.id() << '\n';

	std::cout << s << '\n';
	memory::write(current, address{ &s }, std::tuple<int, int, short, double, double, int>{ 0, 1, 2, 3.3, 4.4, 5 });
	//memory::write(current, address{ &t }, std::tuple<int, const char[16], short, double>{ 2, "ham", 4, 6.6 });
	std::cout << s << '\n';

	const auto ptr = distant::virtual_malloc<int>(current);
	*ptr = 2;

	std::string string = "heyyyyyyyyyyyyyyyyyyyyyyyyyyyyy";

	memory::write(current, address{ string.data() }, std::string{ "hello!" });

	const auto result = memory::read<std::string>(current, address{ string.data() });

	std::cout << "read = " << result << '\n';

	distant::virtual_protect<page_protection::readonly>(current, ptr.get(), sizeof(decltype(ptr)::element_type));
	std::cout << "t = " << ptr << '\n';
	std::cout << "*t = " << *ptr << '\n';
	std::cout << "read = " << distant::memory::read<int>(current, ptr.get()) << '\n';

	distant::virtual_protect<page_protection::readwrite>(current, ptr.get(), sizeof(decltype(ptr)::element_type));
	*ptr = 3;
	std::cout << "*t = " << *ptr << '\n';
	distant::virtual_free(current, ptr);
}
