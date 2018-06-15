// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once
#include <distant/kernel_objects/access_token.hpp>

#include <distant/detail/tags.hpp>

#include <distant/support/winapi/token.hpp>

namespace distant::kernel_objects
{
	namespace detail
	{
		using boost::winapi::DWORD_;
		using boost::winapi::HANDLE_;

		template <typename Object>
		struct dispatcher {};

		template <process_rights A>
		struct dispatcher<process<A>>
		{
			using dispatch = distant::detail::process_tag;
		};

		template <>
		struct dispatcher<unsafe_process>
		{
			using dispatch = distant::detail::process_base_tag;
		};

		inline HANDLE_ get_token_impl(const unsafe_process& process, const DWORD_ access,
		                              const distant::detail::process_base_tag tag) noexcept
		{
			static_cast<void>(tag);
			HANDLE_ token = nullptr;

			OpenProcessToken(process.handle().native_handle(), access, &token);
			return token;
		}

		template <access_rights::process A>
		HANDLE_ get_token_impl(const process<A>& process, const DWORD_ access, const distant::detail::process_tag tag) noexcept
		{
			using access_rights = access_rights::process;

			static_cast<void>(tag);
			return get_token_impl(reinterpret_cast<const unsafe_process&>(process), access, distant::detail::process_base_tag{});
		}

		/*inline HANDLE_ get_token_impl(const kernel_objects::thread_base& thread, DWORD_ access, bool self) noexcept
		{
			boost::winapi::HANDLE_ token = nullptr;

			boost::winapi::OpenThreadToken(thread, access, self, &token);
			return token;
		}*/
	}

	//class access_token
	//public:
	template <access_rights::token A, typename K>
	access_token<A, K>::access_token(const K& k) noexcept
		: Base(
			distant::handle<access_token>{
				detail::get_token_impl(k, static_cast<boost::winapi::DWORD_>(A), detail::dispatcher<K>::dispatch{})
			}) 
	{}

	template <access_rights::token A, typename K>
	bool access_token<A, K>::has_privilege(const security::privilege& p) const noexcept
	{
		if (!p) return false;

		boost::winapi::BOOL_ result = false;
		boost::winapi::PRIVILEGE_SET_ set = p;

		boost::winapi::privilege_check(this->handle().native_handle(), &set, &result);
		return result;
	}

	template <access_rights::token A, typename K>
	bool access_token<A, K>::set_privilege(const security::privilege& p,
	                                       security::privilege::attributes attribute) noexcept
	{
		if (!p) return false;

		boost::winapi::TOKEN_PRIVILEGES_ temp = p;
		temp.Privileges[0].Attributes = static_cast<boost::winapi::DWORD_>(attribute);

		if (boost::winapi::adjust_token_privilege(this->handle().native_handle(), false, &temp, sizeof(temp), nullptr,
		                                          nullptr))
		{
			// If the privilege was set return success.
			if (last_error().value() != boost::winapi::ERROR_NOT_ALL_ASSIGNED_)
				return true;
		}

		// Otherwise an error has occured.
		return false;
	}

	template <access_rights::token A, typename K>
	bool access_token<A, K>::remove_privilege(const security::privilege& p) noexcept
	{
		return this->set_privilege(p, security::privilege::attributes::removed);
	}

//free:
	template <token_rights Access, typename KernelObject, typename>
	access_token<Access, KernelObject>
	get_access_token(const KernelObject& object) noexcept
	{
		return access_token<Access, KernelObject>{object};
	}

	template <typename KernelObject, typename>
	access_token<token_rights::adjust_privileges | token_rights::query, KernelObject>
	get_access_token(const KernelObject& object) noexcept
	{
		return get_access_token<token_rights::adjust_privileges | token_rights::query>(object);
	}

	inline access_token<token_rights::all_access, process<>>
	get_access_token() noexcept
	{
		return get_access_token<token_rights::all_access>(kernel_objects::current_process());
	}
} // namespace distant::kernel_objects
