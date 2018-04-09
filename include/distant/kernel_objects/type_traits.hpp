// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/kernel_objects/fwd.hpp>

namespace distant::kernel_objects
{
	namespace detail
	{
		template <typename>
		struct has_token_access;

		template <process_rights A>
		struct has_token_access<process<A>>
		{
			// The OpenProcessToken API call requires the following access rights.
			static constexpr bool value = check_permission(A, process_rights::query_information);
		};
	}
}
