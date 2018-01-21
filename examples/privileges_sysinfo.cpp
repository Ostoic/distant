#define __cpp_lib_experimental_filesystem
#define BOOST_USE_WINDOWS_H
#define NOMINMAX

#include <distant.hpp>
#include <distant\kernel\securable.hpp>
#include <distant\kernel\access_token.hpp>
#include <distant\security\privileges.hpp>

#include <distant\system\information.hpp>

using access_rights = distant::access_rights::process;
using token_rights = distant::access_rights::token;

constexpr auto flag_stq = access_rights::query_information | access_rights::synchronize | access_rights::terminate;

void typesafe_kill(distant::process<flag_stq>& p)
{
	std::cout << "Killing process " << p.id() << "...\n";
	p.kill();
}

void request_debug_privileges()
{
	namespace privileges = distant::security::privileges;
	auto token = distant::get_access_token(distant::current_process());

	if (token.has_privilege(privileges::debug))
	{
		std::cout
			<< "Debug privileges are enabled...\n"
			<< "Do some magic\n";
	}
	else
	{
		std::cout
			<< "Debug privileges are not enabled!\n"
			<< "We need those!\n";

		if (token.set_privilege(privileges::debug))
			std::cout << "Debug privileges granted!\n";
		else
		{
			std::cerr << "Unable to obtain debug privileges!\n";
			std::cout << distant::last_error() << '\n';
		}
	}
}

int main()
{
	using process = distant::kernel::process_base;

	std::cout << std::boolalpha;

	std::cout << "Process count = " << distant::snapshot<process>{}.get().size() << '\n';
	request_debug_privileges();
	std::cout << "Process count = " << distant::snapshot<process>{}.get().size() << '\n';

	auto current = distant::current_process<>();

	std::cout << "Process is 32bit: " << current.is_32bit() << '\n';

	std::wcout << "Computer Name: " << distant::system::computer_name() << '\n';
	std::wcout << "User Name: " << distant::system::username() << '\n';
	std::wcout << "Windows path: " << distant::system::windows_path() << '\n';

	std::cout << "\nHardware info:\n";
	std::cout << "Page size: " << distant::system::page_size() << '\n';
	std::cout << "Allocation granularity: " << distant::system::allocation_granularity() << '\n';
	std::cout << "Number of Processors: " << distant::system::number_of_processors() << '\n';
	std::cout << "Oem Id: " << distant::system::oem_id() << '\n';
	std::cout << "Processor type: " << distant::system::processor_type() << '\n';

	typesafe_kill(distant::current_process<>());
	std::cout << "End\n";
	return 0;
}
