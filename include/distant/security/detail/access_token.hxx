#pragma once
#include <distant\security\access_token.hpp>

#include <distant\detail\tags.hpp>
#include <distant\detail\attorney.hpp>

#include <distant\support\winapi\token.hpp>
#include <distant\kernel\process.hpp>

#include <boost\winapi\basic_types.hpp>

namespace distant::security
{
	namespace detail
	{
		using boost::winapi::DWORD_;
		using boost::winapi::HANDLE_;

		inline HANDLE_ get_token_impl(const kernel::process_base& process, DWORD_ access) noexcept
		{
			HANDLE_ token = NULL;

			::OpenProcessToken(process.get_handle().native_handle(), access, &token);
			return token;
		}

		/*inline HANDLE_ get_token_impl(distant::handle<> process, DWORD_ access, bool self) noexcept
		{
			boost::winapi::HANDLE_ token = NULL;

			::OpenThreadToken(process, access, self, &token);
			return token;
		}*/
	}

//class access_token
//public:
	template <access_rights::token A, typename K>
	inline access_token<A, K>::access_token(const K& k)
		: m_handle(distant::handle<access_token>(
			detail::get_token_impl(k, static_cast<boost::winapi::DWORD_>(A)))) {}

	template <access_rights::token A, typename K>
	template <access_rights::token OA, typename OK>
	inline access_token<A, K>::access_token(access_token<OA, OK>&& other) noexcept
		: access_token(std::move(reinterpret_cast<access_token<A, K>>(other))) {}


	template <access_rights::token A, typename K>
	inline bool access_token<A, K>::check_privilege(const security::privilege& p) const noexcept
	{
		boost::winapi::BOOL_ result = false;
		boost::winapi::PRIVILEGE_SET_ set = p;

		boost::winapi::privilege_check(m_handle.native_handle(), &set, &result);
		return result;
	}

	template <access_rights::token A, typename K>
	inline void access_token<A, K>::adjust_privilege(const security::privilege& p)
	{
		boost::winapi::TOKEN_PRIVILEGES_ temp = p;

		if (!boost::winapi::adjust_token_privilege(expose::native_handle(m_handle), false, &temp, sizeof(temp), NULL, NULL))
		{
			m_last_error.update();

			if (m_last_error.value() == boost::winapi::ERROR_NOT_ALL_ASSIGNED_)
				throw std::system_error(m_last_error, "The token does not have the specified privilege.");
			else
				throw std::system_error(m_last_error, "Unknown error");
		}
	}

	template <access_rights::token A, typename K>
	inline access_token<A, K>::operator bool() const noexcept
	{
		return static_cast<bool>(m_handle);
	}

//free:
	template <access_rights::token access = access_token::token::all_access, typename KernelObject>
	inline access_token<access, KernelObject> get_access_token(const KernelObject& object)
	{
		return access_token<access, KernelObject>(object);
	}
}
