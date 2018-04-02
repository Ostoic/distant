#pragma once
#include <distant/security/privileges.hpp>

#include <distant/kernel/access_token.hpp>

namespace distant::security::privileges {

	inline bool request_debug_privileges() noexcept
	{
		return distant::get_access_token(distant::current_process<>()).set_privilege(privileges::debug);
	}
}