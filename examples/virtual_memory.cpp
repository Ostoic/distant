// distance dev.cpp : Defines the entry point for the console application.

#include <iostream>
#include <distant/virtual_memory.hpp>

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

int main()
{
	using namespace distant;
	using namespace utility;

	auto current = distant::current_process();

	constexpr std::tuple<int, int, int> tuple = { 1, 2, 7 };

	const auto a = address{ &tuple };

	auto addresses = meta::tuple_transform(tuple, [](const auto& element)
	{
		return address{ std::addressof(element) };
	});

	tuple_print(addresses);
	
	tuple_print(tuple);
	memory::write(current, address{ &tuple }, std::tuple<int, int, int>{ 0, -5, 9});
	tuple_print(tuple);

	const auto ptr = distant::virtual_malloc<int>(current);
	*ptr = 2;

	std::string string = "heyyyyyyyyyyyyyyyyyyyyyyyyyyyyy";

	memory::write(current, address{ string.data() }, std::string{ "hello!" });

	const auto result = memory::read<std::string>(current_process(), address{ string.data() });

	std::cout << "read = " << result << '\n';

	distant::virtual_protect<distant::page_protection::readonly>(current, ptr.get(), sizeof(decltype(ptr)::element_type));
	std::cout << "t = " << ptr << '\n';
	std::cout << "*t = " << *ptr << '\n';
	std::cout << "read = " << distant::memory::read<int>(current, ptr.get()) << '\n';

	distant::virtual_protect<distant::page_protection::readwrite>(current, ptr.get(), sizeof(decltype(ptr)::element_type));
	*ptr = 3;
	std::cout << "*t = " << *ptr << '\n';
	distant::virtual_free(current, ptr);
}
