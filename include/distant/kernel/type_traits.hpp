#pragma once

#include <distant/kernel/fwd.hpp>

namespace distant::kernel
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
