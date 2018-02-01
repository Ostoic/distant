#pragma once
#include <distant\privileges.hpp>

#include <distant\kernel\process.hpp>

namespace distant
{
	inline bool request_debug_privileges() noexcept
	{
		return distant::get_access_token(distant::current_process<>()).set_privilege(distant::privileges::debug);
	}
}
