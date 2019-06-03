#pragma once

#include <distant/agents/fwd.hpp>
#include <distant/type_traits.hpp>

namespace distant::security::detail
{
	template <class Agent>
	constexpr bool has_token_access() noexcept
	{
		using traits = kernel_object_traits<Agent>;
		using access_rights_t = typename traits::access_rights_t;

		return check_permission(traits::access_rights(), access_rights_t::query_information);
	}
}
