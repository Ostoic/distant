#pragma once

#include <distant/types.hpp>
#include <distant/type_traits.hpp>
#include <distant/utility/meta/transformations.hpp>

#include <distant/agents/detail/native_handle_of_dispatch.hpp>

namespace distant::agents
{
	template <class Agent>
	bool is_valid(const Agent& object) noexcept
	{
		return object.valid();
	}

	template <class Agent>
	bool is_active(const Agent& object) noexcept
	{
		return object.is_active();
	}

	template <class Agent>
	constexpr auto native_handle_of(Agent&& object) noexcept
	{
		using utility::meta::remove_cvref;
		return detail::native_handle_impl<remove_cvref<Agent>>
			::native_handle(std::forward<Agent>(object));
	}

	constexpr auto native_handle_of(const boost::winapi::HANDLE_ handle) noexcept
	{
		return handle;
	}

	template <class Agent>
	constexpr auto access_rights_of() noexcept
	{
		using utility::meta::remove_cvref;
		return kernel_object_traits<remove_cvref<Agent>>
			::access_rights();
	}

	template <class Agent>
	auto access_rights_of(const Agent& object) noexcept
	{
		using utility::meta::remove_cvref;
		return kernel_object_traits<remove_cvref<Agent>>
			::access_rights(object);
	}

	template <class Agent>
	constexpr void require_access(const typename kernel_object_traits<Agent>::access_rights_t access) noexcept
	{
		using utility::meta::remove_cvref;
		static_assert(
			check_permission(access_rights_of<remove_cvref<Agent>>(), access),
			"[agents interface.hpp::require_access] Insufficient access"
		);
	}

	template <class Agent>
	uint id_of(const Agent& object) noexcept
	{
		using utility::meta::remove_cvref;
		return kernel_object_traits<remove_cvref<Agent>>::get_id(native_handle_of(object));
	}

	constexpr uint id_of(const uint id) noexcept
	{
		return id;
	}
}

