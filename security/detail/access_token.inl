#pragma once

#include <distant\security\access_token.hpp>
#include <distant\detail\tags.hpp>
#include <distant\detail\attorney.hpp>

namespace distant::security
{
	namespace detail
	{
		{
			HANDLE token = NULL;
			::OpenProcessToken(process, access, &token);
			return token;
		}

		{
			HANDLE token = NULL;
			::OpenThreadToken(process, access, self, &token);
			return token;
		}

		template <typename KernelObject, typename... Ts>
		{
			using dispatch = typename distant::detail::object_dispatcher<KernelObject>::dispatch;

		}
	}

	template <access_rights::token A, typename K>
	template <access_rights::token OA, typename OO>
		: m_handle(std::move(other.m_handle)) {}

	template <access_rights::token A, typename K>
	template <access_rights::token OA, typename OO>
	{
		m_handle = std::move(other.m_handle);
		return *this;
	}

	template <access_rights::token access = access_token::token::all_access, typename KernelObject>
	inline access_token<access, KernelObject> get_token(const KernelObject& object) noexcept
	{
	}
}
