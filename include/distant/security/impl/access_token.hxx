// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once
#include "../access_token.hpp"

#include <distant/detail/tags.hpp>
#include <distant/support/winapi/token.hpp>

namespace distant::security
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
		struct dispatcher<agents::basic_process>
		{
			using dispatch = distant::detail::process_base_tag;
		};

		inline HANDLE_ get_token_impl(const agents::basic_process& process, const DWORD_ access,
		                              const distant::detail::process_base_tag tag) noexcept
		{
			static_cast<void>(tag);
			HANDLE_ token = nullptr;

			::OpenProcessToken(native_handle_of(process), access, &token);
			return token;
		}

		template <access_rights::process A>
		HANDLE_ get_token_impl(const process<A>& process, const DWORD_ access, const distant::detail::process_tag tag) noexcept
		{
			using access_rights = access_rights::process;

			static_cast<void>(tag);
			return get_token_impl(reinterpret_cast<const agents::basic_process&>(process), access, distant::detail::process_base_tag{});
		}

		/*inline HANDLE_ get_token_impl(const agents::thread& thread, DWORD_ access, bool self) noexcept
		{
			boost::winapi::HANDLE_ token = nullptr;

			boost::winapi::OpenThreadToken(native_handle_of(thread), access, self, &token);
			return token;
		}*/

	} // namespace detail

	//class access_token
	//public:
	template <access_rights::token A, typename K>
	access_token<A, K>::access_token(const K& k) noexcept
		: handle_(
			distant::kernel_handle{
				detail::get_token_impl(k, static_cast<boost::winapi::DWORD_>(A), detail::dispatcher<K>::dispatch{})
			}) 
	{}

	template <access_rights::token A, typename K>
	bool access_token<A, K>
		::has_privilege(const security::privilege& p) const noexcept
	{
		if (!p) return false;

		boost::winapi::BOOL_ result = false;
		boost::winapi::PRIVILEGE_SET_ set = p;

		boost::winapi::privilege_check(handle_.native_handle(), &set, &result);
		return result;
	}

	template <access_rights::token A, typename K>
	bool access_token<A, K>
		::set_privilege(const security::privilege& p, security::privilege::attributes attribute) noexcept
	{
		if (!p) return false;

		boost::winapi::TOKEN_PRIVILEGES_ temp = p;
		temp.Privileges[0].Attributes = static_cast<boost::winapi::DWORD_>(attribute);

		if (boost::winapi::adjust_token_privilege(
			handle_.native_handle(), false, &temp, sizeof(temp), nullptr, nullptr))
		{
			// If the privilege was set return success.
			if (last_error().value() != boost::winapi::ERROR_NOT_ALL_ASSIGNED_)
				return true;
		}

		// Otherwise an error has occured.
		return false;
	}

	template <access_rights::token A, typename K>
	bool access_token<A, K>
		::remove_privilege(const security::privilege& p) noexcept
	{
		return this->set_privilege(p, security::privilege::attributes::removed);
	}

//free:
	template <token_rights Access, typename Agent>
	access_token<Access, Agent>
		primary_access_token(const Agent& object) noexcept
	{
		static_assert(
			detail::has_token_access<Agent>(),
			"[primary_access_token] insuficient access to Agent"
		);

		return access_token<Access, Agent>{object};
	}

	template <typename Agent>
	access_token<token_rights::adjust_privileges | token_rights::query, Agent>
		primary_access_token(const Agent& object) noexcept
	{
		return primary_access_token<token_rights::adjust_privileges | token_rights::query>(object);
	}

	inline access_token<token_rights::all_access, process<>>
		primary_access_token() noexcept
	{
		return primary_access_token<token_rights::all_access>(agents::current_process());
	}

} // namespace distant::security
