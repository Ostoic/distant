#pragma once

#include <distant/access_rights.hpp>


namespace distant::memory
{
	namespace detail
	{
		constexpr bool has_virtual_malloc_support(const page_protection protection) noexcept
		{
			switch (protection)
			{
				case page_protection::execute_writecopy:
					return false;

				case page_protection::writecopy:
					return false;

				case page_protection::noaccess:
					return false;

				case page_protection::guard:
					return false;

				case page_protection::nocache:
					return false;

				case page_protection::writecombine:
					return false;

				default:
					return true;
			}
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
