// distance dev.cpp : Defines the entry point for the console application.

#include <iostream>

#include <distant/virtual_memory.hpp>

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

int main()
{
	std::integral_constant<unsigned int, 5>{};

	using namespace distant;
	using namespace utility;
	using memory::morphism;

	auto current = distant::current_process();

	S s = { 1, 2, 3, 6.1, 5.5, 0 };

	std::cout << s << '\n';
	memory::write(current, address{ &s }, std::tuple<int, int, morphism<std::string, const char*>>{ 0, 1, "hello"});
	std::cout << s << '\n';

	const auto ptr = distant::virtual_malloc<int>(current);
	*ptr = 2;

	std::string string = "heyyyyyyyyyyyyyyyyyyyyyyyyyyyyy";

	memory::write(current, address{ string.data() }, std::string{ "hello!" });

	const auto result = memory::read<std::string>(current_process(), address{ string.data() });

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
