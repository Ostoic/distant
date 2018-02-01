// distance dev.cpp : Defines the entry point for the console application.
#define __cpp_lib_experimental_filesystem
#define BOOST_USE_WINDOWS_H
#define NOMINMAX

#include <iostream>

#include <distant.hpp>
#include <distant\privileges.hpp>

void enable_debug_privileges()
{
	// Retrieve the primary access token for the current process.
	auto token = distant::get_access_token();

	// Check if debug privileges are enabled for the current process.
	if (token.has_privilege(distant::privileges::debug))
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

		// Attempt to enable debug privileges...
		if (token.set_privilege(distant::privileges::debug))
			std::cout << "Debug privileges granted!\n";
		else
			std::cerr
				<< "Unable to obtain debug privileges!\n"
				<< distant::last_error() << '\n';
	}
}

void remove_debug_privileges()
{
	// Retrieve the primary access token for the current process.
	auto token = distant::get_access_token();

	// Check if debug privileges are enabled for the current process.
	if (token.has_privilege(distant::privileges::debug))
	{
		std::cout << "Removing debug privileges now...\n";

		// Attempt to remove debug privileges...
		if (token.remove_privilege(distant::privileges::debug))
		{
			std::cout << "Successfully removed debug privileges!\n";
			std::cout << token.has_privilege(distant::privileges::debug) << '\n';
		}
		else
			std::cerr
				<< "Unable to obtain debug privileges!\n"
				<< distant::last_error() << '\n';
	}
}

int main()
{
	using process = distant::process<>;

	std::cout << std::boolalpha;

	// Displaying the number of processes we have full access to after each access token modification shows
	// whether or not our modification has been successful or not. This is a verification that the access token functions
	// work as expected.
	std::cout << "Process count before debug privileges = " << distant::snapshot<process>{}.get().size() << '\n';
	enable_debug_privileges();
	std::cout << "Process count after enabling debug privileges = " << distant::snapshot<process>{}.get().size() << '\n';
	remove_debug_privileges();
	std::cout << "Process count after removing debug privileges = " << distant::snapshot<process>{}.get().size() << "\n\n";


	std::cout << "Processes that have debug privileges:\n";

	for (const auto p : distant::snapshot<process>{})
	{
		std::wcout
			<< "Process " << p.id() << ": " << p.filename() << '\n'
			<< "Has debug privileges: " << distant::get_access_token(p).has_privilege(distant::privileges::debug) << "\n\n";
	}
	return 0;
}