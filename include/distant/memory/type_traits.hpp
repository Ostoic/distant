// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/access_rights.hpp>


namespace distant::memory
{
	namespace detail
	{
		constexpr bool has_virtual_malloc_support(const page_protection protection) noexcept
		{
			if (protection >= page_protection::execute_writecopy)
				return false;
			if (protection >= page_protection::writecopy)
				return false;
			if (protection >= page_protection::noaccess)
				return false;
			if (protection >= page_protection::guard)
				return false;
			if (protection >= page_protection::nocache)
				return false;
			if (protection >= page_protection::writecombine)
				return false;
			return true;
		}

		constexpr bool has_virtual_protect_support(const page_protection) noexcept
		{ return true; }

		constexpr bool has_virtual_free_support(const page_protection) noexcept
		{ return true; }

		template <typename Element>
		struct required_vm_access
		{
			static constexpr auto value =
				(std::is_const<Element>::value) ? distant::vm_read : distant::vm_rw_op;
		};

		template <typename Element>
		constexpr auto required_vm_access_v = required_vm_access<Element>::value;

	} // namespace detail

	template <typename Virtual>
	struct virtual_traits;

} // namespace distant::memory
