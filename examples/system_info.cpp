#include <distant.hpp>

#include <iostream>

int main()
{
	// Basic system information
	std::wcout
		<< "Computer Name: " << distant::system::computer_name() << '\n'
		<< "User Name: "	 << distant::system::username() << '\n'
		<< "Windows path: "  << distant::system::windows_path() << '\n';

	std::cout
		<< "\nHardware info:\n"
		<< "Page size: "			  << distant::system::page_size() << '\n'
		<< "Allocation granularity: " << distant::system::allocation_granularity() << '\n'
		<< "Number of Processors: "   << distant::system::number_of_processors() << '\n'
		<< "Oem Id: "				  << distant::system::oem_id() << '\n'
		<< "Processor type: "		  << distant::system::processor_type() << '\n';
}