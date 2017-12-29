// distance dev.cpp : Defines the entry point for the console application.
#include "targetver.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <string_view>

#define __cpp_lib_experimental_filesystem

#include <distant.hpp>
#include <distant\kernel\securable.hpp>
#include <distant\security\access_token.hpp>

using access_rights = distant::access_rights::process;
using token_rights = distant::access_rights::token;

/*
BOOLEAN ucmPrivilegeEnabled(
_In_ HANDLE hToken,
_In_ ULONG Privilege
)
{
NTSTATUS status;
PRIVILEGE_SET Privs;
BOOLEAN bResult = FALSE;

Privs.Control = PRIVILEGE_SET_ALL_NECESSARY;
Privs.PrivilegeCount = 1;
Privs.Privilege[0].Luid.LowPart = Privilege;
Privs.Privilege[0].Luid.HighPart = 0;
Privs.Privilege[0].Attributes = SE_PRIVILEGE_ENABLED_BY_DEFAULT | SE_PRIVILEGE_ENABLED;

status = PrivilegeCheck(hToken, &Privs, &bResult);
RtlSetLastWin32Error(RtlNtStatusToDosError(status));

return bResult;
}
*/

void request_debug_privileges(const distant::kernel::process_base& process)
{
	auto token = distant::security::get_access_token<token_rights::adjust_privileges>(process);

	if (token)
	{
		distant::security::privilege debug(SE_DEBUG_NAME);
		if (token.check_privilege(debug))
		{
			token.adjust_privilege(debug);
			std::cout << "Debug privileges acquired for process: " << process.filename() << "\n";
		}
	}
	else
		std::cout << "Unable to adjust privileges for the given process.\n";
}

int main()
{
	using process = distant::process<>;
	const auto current = distant::current_process();

	const auto& handle1 = current.get_handle();
	const auto& handle2 = current.get_handle<process>();

	std::cout << "Equals: " << (handle1 == handle2) << '\n';

	try
	{
		request_debug_privileges(current);
	}
	catch (std::system_error& e)
	{
		std::cout << "System error " << e.code().category().name() << ": " << e.what() << '\n';
	}

	std::cout << "Successfully set debug privileges for " << current.filename() << '\n';

	//std::cin.get();
	return 0;
}
