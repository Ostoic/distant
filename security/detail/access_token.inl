#pragma once

#include <distant\security\access_token.hpp>
#include <distant\detail\tags.hpp>
#include <distant\detail\attorney.hpp>

namespace distant::security
{
	namespace detail
	{
		inline HANDLE get_token_dispatch(distant::detail::process_tag, HANDLE process, DWORD access)
		{
			HANDLE token = NULL;
			::OpenProcessToken(process, access, &token);
			return token;
		}

		inline HANDLE get_token_dispatch(distant::detail::thread_tag, HANDLE process, DWORD access, bool self)
		{
			HANDLE token = NULL;
			::OpenThreadToken(process, access, self, &token);
			return token;
		}

		/*template <typename KernelObject>
		inline HANDLE get_token_impl(HANDLE object, DWORD access)
		{
			using dispatch = typename distant::detail::object_dispatcher<KernelObject>::dispatch;

			return get_token_dispatch(dispatch(), object, access);
		}*/

		template <typename KernelObject, typename... Ts>
		inline HANDLE get_token_impl(HANDLE object, DWORD access, Ts&&... ts)
		{
			using dispatch = typename distant::detail::object_dispatcher<KernelObject>::dispatch;

			return get_token_dispatch(dispatch(), object, access, std::forward<Ts>(ts)...);
		}
	}

//class access_token
//public:
	template <access_rights::token A, typename K>
	inline access_token<A, K>::access_token(const K& k)
		: m_handle(distant::handle<access_token>(
			detail::get_token_impl<K>(
				expose::native_handle(k.get_handle()), static_cast<DWORD>(A)))) {}

	// Bivariant move constructor
	template <access_rights::token A, typename K>
	template <access_rights::token OA, typename OO>
	inline access_token<A, K>::access_token(access_token<OA, OO>&& other)
		: m_handle(std::move(other.m_handle)) {}

	// Bivariant move assignment
	template <access_rights::token A, typename K>
	template <access_rights::token OA, typename OO>
	inline access_token<A, K>& access_token<A, K>::operator=(access_token<OA, OO>&& other)
	{
		m_handle = std::move(other.m_handle);
		return *this;
	}

	template <access_rights::token A, typename K>
	inline void access_token<A, K>::adjust() // privileges as argument
	{

	}

//free:
	template <access_rights::token access = access_token::token::all_access, typename KernelObject>
	inline access_token<access, KernelObject> get_token(const KernelObject& object) noexcept
	{
		return access_token<access, KernelObject>(object);
	}
}
