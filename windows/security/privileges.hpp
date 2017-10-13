#pragma once

#include <string_view>
#include <winnt.h>

namespace distant::windows::security::privileges
{
	constexpr auto debug = SE_DEBUG_NAME;
	constexpr auto impersonate = SE_IMPERSONATE_NAME;
}