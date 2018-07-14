#include <distant.hpp>

#include <iostream>

void* operator new(const std::size_t sz)
{
	std::cout << "[NEW] new operator called\n";
	return std::malloc(sz);
}

int main()
{
	// Basic system information
	std::wcout
		<< "Computer Name: " << distant::system::computer_name() << '\n'
		<< "User Name: "	 << distant::system::username() << '\n'
		<< "Windows path: "  << distant::system::windows_path() << '\n';

	std::wcout
		<< "\nHardware info:\n"
		<< "Page size: "			  << distant::system::page_size() << '\n'
		<< "Allocation granularity: " << distant::system::allocation_granularity() << '\n'
		<< "Number of Processors: "   << distant::system::number_of_processors() << '\n'
		<< "Processor Architecture: " << distant::system::architecture() << '\n'
		<< "Processor type: "		  << distant::system::processor_type() << '\n';

	distant::system::performance_info system_performance;

	//std::cout << "Usermode process count = " << distant::snapshot<distant::unsafe_process>{}.as<std::vector>().size() << '\n';
	std::cout << "Process count = " << system_performance.process_count() << '\n';
	std::cout << "Thread count = " << system_performance.thread_count() << '\n';
	std::cout << "(Page size, Page size) = (" << system_performance.page_size() << ", " << distant::system::page_size() << ")\n";
}