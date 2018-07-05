#pragma once

#include <distant/kernel_objects/fwd.hpp>
#include <distant/type_traits.hpp>

namespace distant::kernel_objects::detail
{
	template <class KernelObject>
	constexpr bool has_token_access() noexcept
	{
		using traits = kernel_object_traits<KernelObject>;
		using access_rights_t = typename traits::access_rights_t;

		return check_permission(traits::access_rights(), access_rights_t::query_information);
	}
}
