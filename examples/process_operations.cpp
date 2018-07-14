// Visual studio doesn't define this macro despite the face that
// the filesystem library is included in the current version
// I don't feel like linking in boost::filesystem.
#define __cpp_lib_experimental_filesystem

#include <string_view>
#include <algorithm>
#include <iostream>

#include <distant.hpp>

int new_count = 0;

void* operator new(const std::size_t sz)
{
	new_count++;
	return std::malloc(sz);
}

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
	std::cout << "Handle value = " << p.handle().native_handle() << '\n';
	std::cout << "Process " << p.id() << '\n';
	std::cout << "State: " << active_string << '\n';
	std::wcout << "Name: " << p.name() << '\n';
	std::wcout << "File Path: " << p.file_path() << '\n';
}

auto find_process(std::wstring_view name)
{
	using process = distant::process<retrieve_path_flags>;
	distant::snapshot<process> snapshot;

	const auto it = std::find_if(snapshot.begin(), snapshot.end(), [&name](const auto& p)
	{
		return p.name() == name;
	});

	if (it != snapshot.end()) return *it;

	return process{};
}

void test()
{
	const std::wstring process_name = L"Taskmgr.exe";
	std::wcout << "Looking for " << process_name << "...\n";

	const auto process = find_process(process_name);

	if (process)
	{
		std::cout << "Found it!\n\n";
		//display_info(process);
	}
	else
		std::wcout << "Could not find " << process_name << "!\n";

	//std::cin.ignore();
	// Kill the current process.
	// Note that distant::current_process() returns the type-unsafe version of distant::process, namely process_base,
	// whereas distant::current_process<>() denotes the function templated version with default access rights (all access).
	//typesafe_kill(distant::current_process());
	//std::cout << "No one should see this\n";
}

int main()
{
	test();

	//try
	//{
	//	//for (int i = 0; i < 1000; ++i)
	//		//test();
	//}
	//catch (const distant::winapi_error& e)
	//{
	//	std::cout << "winapi_error " << e << '\n';
	//}
	//catch (const std::exception& e)
	//{
	//	std::cout << "Exception occured: " << e.what() << '\n';
	//}

	std::cout << "operator new called " << new_count << " times\n";
	return 0;
}