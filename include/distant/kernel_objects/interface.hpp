#pragma once

#include <distant/type_traits.hpp>
#include <distant/utility/meta/transformations.hpp>

#include <distant/kernel_objects/detail/native_handle_of_dispatch.hpp>

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
	struct native_handle_traits
		: detail::native_handle_impl<utility::meta::remove_cvref<KernelObject>>
	{
		using detail::native_handle_impl<utility::meta::remove_cvref<KernelObject>>::native_handle;
	};

	template <class KernelObject>
	constexpr auto native_handle_of(KernelObject&& object) noexcept
	{
		return native_handle_traits<KernelObject>::native_handle(std::forward<KernelObject>(object));
	}

	template <class KernelObject>
	constexpr auto access_rights_of() noexcept
	{
		using utility::meta::remove_cvref;
		return kernel_object_traits<remove_cvref<KernelObject>>::access_rights();
	}

	template <class KernelObject>
	auto access_rights_of(const KernelObject& object) noexcept
	{
		using utility::meta::remove_cvref;
		return kernel_object_traits<remove_cvref<KernelObject>>::access_rights(object);
	}

	template <class KernelObject>
	constexpr void require_access(const typename kernel_object_traits<KernelObject>::access_rights_t access) noexcept
	{
		using utility::meta::remove_cvref;
		static_assert(
			check_permission(access_rights_of<remove_cvref<KernelObject>>(), access),
			"[kernel_objects interface.hpp::require_access] Insufficient access"
		);
	}

	template <class KernelObject>
	uint id_of(const KernelObject& object) noexcept
	{
		using utility::meta::remove_cvref;
		return kernel_object_traits<remove_cvref<KernelObject>>::get_id(native_handle_of(object));
	}

	constexpr uint id_of(const uint id) noexcept
	{
		return id;
	}
}

