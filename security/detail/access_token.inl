#pragma once

#include <distant\security\access_token.hpp>
#include <distant\detail\tags.hpp>
#include <distant\detail\attorney.hpp>

namespace distant::security
{
	namespace detail
	{
		HANDLE get_token_impl(distant::detail::process_tag, HANDLE process, DWORD access)
		{
			HANDLE token = NULL;
			::OpenProcessToken(process, access, &token);
			return token;
		}

		HANDLE get_token_impl(distant::detail::thread_tag, HANDLE process, DWORD access, bool self)
		{
			HANDLE token = NULL;
			::OpenThreadToken(process, access, self, &token);
			return token;
		}

		template <typename KernelObject, typename... Ts>
		HANDLE get_token_impl(HANDLE object, DWORD access, Ts&&... ts)
		{
			using dispatch = typename distant::detail::object_dispatcher<KernelObject>::dispatch;

			return access_token(handle(
					get_token_impl(dispatch(), object, access, std::forward<Ts>(ts)...)));
		}
	}

	template <access_rights::token A, typename K>
	template <access_rights::token OA, typename OO>
	access_token<A, K>::access_token(const access_token<OA, OO>&& other)
		: m_handle(std::move(other.m_handle)) {}

	template <access_rights::token A, typename K>
	template <access_rights::token OA, typename OO>
	access_token<A, K>& access_token<A, K>::operator=(access_token<OA, OO>&& other)
	{
		m_handle = std::move(other.m_handle);
		return *this;
	}

	template <access_rights::token access = access_token::token::all_access, typename KernelObject>
	inline access_token<access, KernelObject> get_token(const KernelObject& object) noexcept
	{
		const auto& handle = object.get_handle();

		//object.
		//detail::get_token_impl()
		return {};
	}
}
