// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once
#include <distant/security/privileges.hpp>

#include <distant/kernel_objects/access_token.hpp>

namespace distant::security::privileges {

	inline bool request_debug_privileges() noexcept
	{
		return distant::primary_access_token(distant::current_process()).set_privilege(privileges::debug);
	}
}