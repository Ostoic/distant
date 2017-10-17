#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <windows.h>
#include <strsafe.h>

#include <WinError.h>
#include <string>

#include <distant\detail\fwd.hpp>

namespace distant::error {

inline std::string format(DWORD error_code)
{
	// Retrieve the system error message for the last-error code
	LPSTR messageBuffer = NULL;

	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM	   |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		error_code,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&messageBuffer,
		0, NULL);

	std::string message(messageBuffer);
	LocalFree(messageBuffer);
	return { message };
}

} // end namespace distant::error