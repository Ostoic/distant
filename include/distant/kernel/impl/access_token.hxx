#pragma once
#include <distant\kernel\access_token.hpp>

#include <distant\detail\tags.hpp>
#include <distant\detail\attorney.hpp>

#include <distant\support\winapi\token.hpp>
#include <distant\kernel\process.hpp>

#include <boost\winapi\basic_types.hpp>

namespace distant::kernel
{
	namespace detail
	{
		using boost::winapi::DWORD_;
		using boost::winapi::HANDLE_;

		inline HANDLE_ get_token_impl(const kernel::process_base& process, DWORD_ access) noexcept
		{
			HANDLE_ token = nullptr;

			boost::winapi::OpenProcessToken(process.get_handle().native_handle(), access, &token);
			return token;
		}

		/*inline HANDLE_ get_token_impl(const kernel::thread_base& thread, DWORD_ access, bool self) noexcept
		{
			boost::winapi::HANDLE_ token = nullptr;

			boost::winapi::OpenThreadToken(thread, access, self, &token);
			return token;
		}*/
	}

//class access_token
//public:
	template <access_rights::token A, typename K>
	inline access_token<A, K>::access_token(const K& k) noexcept 
		: Base(distant::handle<access_token>(
			detail::get_token_impl(k, static_cast<boost::winapi::DWORD_>(A)))) {}

	template <access_rights::token A, typename K>
	inline bool access_token<A, K>::has_privilege(const security::privilege& p) const noexcept
	{
		if (!p) return false;
		boost::winapi::BOOL_ result = false;
		boost::winapi::PRIVILEGE_SET_ set = p;

		boost::winapi::privilege_check(this->get_handle().native_handle(), &set, &result);
		return result;
	}

	template <access_rights::token A, typename K>
	inline bool access_token<A, K>::set_privilege(const security::privilege& p, security::privilege::attributes attribute) noexcept
	{
		if (!p) return false;

		boost::winapi::TOKEN_PRIVILEGES_ temp = p;
		temp.Privileges[0].Attributes = static_cast<boost::winapi::DWORD_>(attribute);

		if (boost::winapi::adjust_token_privilege(this->get_handle().native_handle(), false, &temp, sizeof(temp), nullptr, nullptr))
		{
			if (distant::last_error().value() != boost::winapi::ERROR_NOT_ALL_ASSIGNED_)
				return true;
				//throw std::system_error(error, "[access_token::set_privilege] The token does not have the specified privilege.");
			//else
				//throw std::system_error(error, "Unknown error");
		}

		return false;
	}

	template <access_rights::token A, typename K>
	inline bool access_token<A, K>::remove_privilege(const security::privilege& p) noexcept
	{
		return this->set_privilege(p, security::privilege::attributes::removed);
	}

//free:
	template <access_rights::token access, typename KernelObject>
	inline access_token<access, KernelObject> get_access_token(const KernelObject& object) noexcept
	{
		return access_token<access, KernelObject>{object};
	}

	template <typename KernelObject>
	inline access_token<access_rights::token::adjust_privileges, KernelObject> get_access_token(const KernelObject& object) noexcept
	{
		return get_access_token<access_rights::token::adjust_privileges>(object);
	}
}
