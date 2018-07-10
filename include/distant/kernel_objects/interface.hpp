#pragma once

#include <distant/type_traits.hpp>

namespace distant::kernel_objects
{
	template <class KernelObject>
	bool is_valid(const KernelObject& object) noexcept
	{
		return object.valid();
	}

	template <class KernelObject>
	bool is_active(const KernelObject& object) noexcept
	{
		return object.is_active();
	}

	template <class KernelObject>
	auto native_handle_of(const KernelObject& object) noexcept
	{
		return object.handle().native_handle();
	}

	template <class KernelObject>
	constexpr auto access_rights_of() noexcept
	{
		return kernel_object_traits<KernelObject>::access_rights();
	}

	template <class KernelObject>
	auto access_rights_of(const KernelObject& object) noexcept
	{
		return kernel_object_traits<KernelObject>::access_rights(object);
	}

	template <class KernelObject>
	constexpr void require_access(const typename kernel_object_traits<KernelObject>::access_rights_t access) noexcept
	{
		static_assert(
			check_permission(access_rights_of<KernelObject>(), access),
			"[kernel_objects interface.hpp::require_access] Insufficient access"
		);
	}

	template <class KernelObject>
	uint id_of(const KernelObject& object) noexcept
	{
		return kernel_object_traits<KernelObject>::get_id(native_handle_of(object));
	}

	constexpr uint id_of(const uint id) noexcept
	{
		return id;
	}
}

