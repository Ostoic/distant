// distance dev.cpp : Defines the entry point for the console application.
#include "targetver.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <string_view>

#define __cpp_lib_experimental_filesystem
#define BOOST_USE_WINDOWS_H
#define NOMINMAX

#include <distant.hpp>
#include <distant\kernel\securable.hpp>
#include <distant\security\access_token.hpp>

#include <Stopwatch.hpp>

using access_rights = distant::access_rights::process;
using token_rights = distant::access_rights::token;

//void request_debug_privileges(const distant::kernel::process_base& process)
//{
//	auto token = distant::security::get_access_token<token_rights::adjust_privileges>(process);
//
//	if (token)
//	{
//		distant::security::privilege debug("SeDebugPrivilege");
//		debug.attribute = distant::security::privilege::attributes::enabled;
//
//		token.set_privilege(debug);
//
//		if (token.is_enabled(debug))
//			std::wcout << "Debug privileges granted.\n";
//		else
//			std::cout << "Debug privilege was not enabled\n";
//	}
//	else
//		std::cout << "Unable to adjust privileges for the given process.\n";
//}

int test_distant()
{
	distant::snapshot<distant::process<>> processList;

	int count = 0;

	for (const auto process : processList)
		if (process)
			count++;

	return count;
}

int test_toolhelp()
{
	int count = 0;

	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(entry);

	HANDLE snapHandle = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapHandle == INVALID_HANDLE_VALUE)
		return 0;

	if (!::Process32First(snapHandle, &entry))
	{
		::CloseHandle(snapHandle);
		return 0;
	}

	do
	{
		HANDLE handle = ::OpenProcess(PROCESS_ALL_ACCESS, false, entry.th32ProcessID);

		if (handle)
		{
			count++;
			CloseHandle(handle);
		}

	} while (::Process32Next(snapHandle, &entry));
	::CloseHandle(snapHandle);
	return count;
}

int test_distant_pidb()
{
	int count = 0;

	std::cout << "Max = " << std::numeric_limits<DWORD>::max() << '\n';
	for (int pid = 4; pid <= 0x4E1C; pid += 4)
	{
		const distant::process<> process(pid);

		if (process)
			count++;
	}

	return count;
}

int test_pidb()
{
	int count = 0;
	for (int pid = 4; pid <= 0x4E1C; pid += 4)
	{
		HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
		
		if (handle && GetProcessVersion(pid) > 0)
		{
			count++;
			CloseHandle(handle);
		}
	}

	return count;
}

int main()
{
	using distant::process;

	const auto current = distant::current_process();
	std::cout << current.is_emulated() << '\n';
	std::cout << std::boolalpha;

	try
	{
		Stopwatch timerDistant, timerDistantPidb, timerPidb, timerTH;

		timerPidb.start();
		int pidbCount = test_pidb();
		timerPidb.stop();

		timerDistant.start();
		int distantCount = test_distant();
		timerDistant.stop();

		timerTH.start();
		int toolhelpCount = test_toolhelp();
		timerTH.stop();

		timerDistantPidb.start();
		int distantPidbCount = test_distant_pidb();
		timerDistantPidb.stop();

		std::cout << "Results:\n";
		std::cout << "Distant: " << distantCount << " handles in " << timerDistant.microseconds() << "us\n";
		std::cout << "Distant Pidb: " << distantPidbCount << " handles in " << timerDistantPidb.microseconds() << "us\n";
		std::cout << "Pidb: " << pidbCount << " handles in " << timerPidb.microseconds() << "us\n";
		std::cout << "TH: " << toolhelpCount << " handles in " << timerTH.microseconds() << "us\n";

	}
	catch (std::system_error& e)
	{
		std::cerr << e.what() << " (" << e.code() << ")\n";
	}

	//for (auto process : processList)
	//{
	//	// Ensure we attached with sufficient access rights.
	//	if (process)
	//	{
	//		std::cout << "Process " << process.id() << '\n';
	//		std::wcout << "Name: " << process.filename() << '\n';
	//		std::wcout << "File Path: " << process.file_path() << '\n';
	//		std::cout << "Active: " << process.is_active() << '\n';

	//		std::cout << "Requesting debug privileges...\n";
	//		try
	//		{
	//			request_debug_privileges(process);
	//		}
	//		catch (std::system_error& e)
	//		{
	//			std::cout << "Error requesting debug privileges: " << e.what() << "(" << e.code() << ")\n";
	//		}

	//		std::cout << '\n';
	//	}
	//}
	//std::cin.get();
	return 0;
}
