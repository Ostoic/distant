#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <string>

#include <boost\winapi\basic_types.hpp>
#include <boost\winapi\error_handling.hpp>
#include <boost\winapi\local_memory.hpp>

#include <distant\detail\fwd.hpp>

namespace distant::error {

inline std::string format(boost::winapi::DWORD_ error_code)
{
	namespace winapi = boost::winapi;
	// Retrieve the system error message for the last-error code
	LPSTR messageBuffer = NULL;

	winapi::format_message(
		winapi::FORMAT_MESSAGE_ALLOCATE_BUFFER_ |
		winapi::FORMAT_MESSAGE_FROM_SYSTEM_	    |
		winapi::FORMAT_MESSAGE_IGNORE_INSERTS_,
		NULL,
		error_code,
		winapi::MAKELANGID_(winapi::LANG_NEUTRAL_, winapi::SUBLANG_DEFAULT_),
		(LPSTR)&messageBuffer,
		0, NULL);

	std::string message(messageBuffer);
	winapi::LocalFree(messageBuffer);
	return { message };
}

} // end namespace distant::error
