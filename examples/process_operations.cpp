// Visual studio doesn't define this macro despite the face that
// the filesystem library is included in the current version
// I don't feel like linking in boost::filesystem.
#define __cpp_lib_experimental_filesystem

#include <string_view>
#include <algorithm>
#include <iostream>

#include <distant.hpp>

constexpr auto terminate_flags =
distant::process_rights::terminate;

// Access rights required to retrieve the executable image's file path.
constexpr auto retrieve_path_flags =
	distant::process_rights::vm_read	 | 
	distant::process_rights::synchronize | 
	distant::process_rights::query_information | distant::process_rights::query_limited_information;

// By specifying the function parameter's access rights, this forces any process passed to at least
// have the required access rights. distant::process performs any necessary permission checks.
void typesafe_kill(distant::process<terminate_flags>& p)
{
	p.kill();
}

void display_info(const distant::process<retrieve_path_flags>& p)
{
	const std::string_view active_string = (p.is_active()) ? "Active" : "Inactive";

	// Display process information
	std::cout << "Process " << p.id() << '\n';
	std::cout << "State: " << active_string << '\n';
	std::wcout << "Name: " << p.name() << '\n';
	std::wcout << "File Path: " << p.file_path() << '\n';
}

auto find_process(std::wstring_view name)
{
	using process = distant::process<retrieve_path_flags>;
	distant::snapshot<process> snapshot;

	if (snapshot)
	{
		const auto it = std::find_if(snapshot.begin(), snapshot.end(), [&](const auto p)
		{
			return p.name() == name;
		});

		if (it != snapshot.end()) return *it;
	}

	return process{};
}

int main()
{
	const std::wstring process_name = L"Taskmgr.exe";
	std::wcout << "Looking for " << process_name << "...\n";

	try
	{
		const auto process = find_process(process_name);

		if (process)
		{
			std::cout << "Found it!\n\n";
			display_info(process);
		}
		else
			std::wcout << "Could not find " << process_name << "!\n";
	}
	catch (std::system_error& e)
	{
		std::cout << "System error " << e.code() << " occured: " << e.what() << '\n';
	}
	catch (std::exception& e)
	{
		std::cout << "Exception occured: " << e.what() << '\n';
	}

	// Kill the current process.
	// Note that distant::current_process() returns the type-unsafe version of distant::process, namely process_base,
	// whereas distant::current_process<>() denotes the function templated version with default access rights (all access).
	typesafe_kill(distant::current_process<>());
	std::cout << "No one should see this\n";
}