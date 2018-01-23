// distance dev.cpp : Defines the entry point for the console application.
#include <iostream>
#include <vector>
#include <algorithm>
#include <string_view>

#define __cpp_lib_experimental_filesystem
#define BOOST_USE_WINDOWS_H
#define NOMINMAX

#include <distant.hpp>

using access_rights = distant::access_rights::process;
using token_rights = distant::access_rights::token;

int test_distant()
{
	distant::snapshot<distant::process<>> processList;

	int count = 0;

	for (const auto process : processList)
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
	catch (std::exception& e)
	{
		std::cout << e.what() << '\n';
	}

	return 0;
}
