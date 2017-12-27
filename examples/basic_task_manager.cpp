// Visual studio doesn't define this macro despite the face that
// the filesystem library is included in the current version
// I don't feel like linking in boost::filesystem.
#define __cpp_lib_experimental_filesystem

#include <string_view>
#include <algorithm>

#include <distant.hpp>

template <distant::access_rights::process T>
void display_info(const distant::process<T>& p)
{
	if (p)
	{
		std::string_view active_string = (p.is_active()) ? "Active" : "Inactive";

		// Process information
		std::cout << "Process " << p.id() << '\n';
		std::cout << "State: " << active_string << '\n';
		std::cout << "Name: " << p.filename() << '\n';
		std::cout << "File Path: " << p.file_path() << '\n';

		// XX Consider memory_status(process) instead of p.memory_status
		const auto ms = p.memory_status();
		std::cout << "Private Usage: " << ms.private_usage() << " kb \n";
		std::cout << "Peak Private Usage: " << ms.peak_private_usage() << " kb \n";

		std::cout << "Working Set: " << ms.working_set() << " kb \n";
		std::cout << "Peak Working Set: " << ms.peak_working_set() << " kb \n";

		std::cout << "Number of Page Faults: " << ms.page_fault_count() << '\n';
		std::cout << "Number of Active Handles: " << ms.handle_count() << '\n';
	}
	else
		std::cout << "An error occured while opening process " << p.id() << '\n';

	// Output a formatted last error message
	std::cout << "Last error: " << p.get_last_error() << '\n' << '\n';
}

auto find_process(std::string_view name)
{
	using access_rights = distant::access_rights::process;

	constexpr auto access = access_rights::vm_read | access_rights::query_information | access_rights::synchronize;

	using process = distant::process<access>;

	distant::system::snapshot<process> snapshot;

	if (snapshot)
	{
		const auto it = std::find_if(snapshot.begin(), snapshot.end(), [&](const auto p)
		{
			return p.filename() == name;
		});

		if (it != snapshot.end()) return *it;
	}

	return process{};
}

void basic_process_operations()
{
	std::cout << "Looking for Wow.exe...\n";

	try
	{
		const auto wow = find_process("Wow.exe");

		if (!wow)
		{
			std::cout << "Could not find WoW.exe!\n";
			return;
		}

		std::cout << "Found it!\n\n";
		display_info(wow);
	}
	catch (std::system_error& e)
	{
		std::cout << "System error " << e.code() << " occured: " << e.what() << '\n';
	}
	catch (std::exception& e)
	{
		std::cout << "Exception occured: " << e.what() << '\n';
	}
}