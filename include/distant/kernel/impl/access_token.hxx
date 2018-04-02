#pragma once
#include <distant/kernel/access_token.hpp>

#include <distant/detail/tags.hpp>
#include <distant/detail/attorney.hpp>

#include <distant/support/winapi/token.hpp>

namespace distant::kernel
{
	namespace detail
	{
		using boost::winapi::DWORD_;
		using boost::winapi::HANDLE_;

		template <typename Object>
		struct dispatcher {};

		template <access_rights::process A>
		struct dispatcher<process<A>>
		{
			using dispatch = distant::detail::process_tag;
		};

		template <>
		struct dispatcher<process_base>
		{
			using dispatch = distant::detail::process_base_tag;
		};

		inline HANDLE_ get_token_impl(const process_base& process, DWORD_ access,
		                              const distant::detail::process_base_tag tag) noexcept
		{
			static_cast<void>(tag);
			HANDLE_ token = nullptr;

			OpenProcessToken(process.get_handle().native_handle(), access, &token);
			return token;
		}

		template <access_rights::process A>
		HANDLE_ get_token_impl(const process<A>& process, DWORD_ access, const distant::detail::process_tag tag) noexcept
		{
			using access_rights = access_rights::process;

			// The OpenProcessToken API call requires the following access rights.
			static_assert(
				check_permission(A, access_rights::query_information),
				" [kernel::get_access_token] Invalid access rights: "
				"Process must have query_information access rights");

			static_cast<void>(tag);
			return get_token_impl(reinterpret_cast<const process_base&>(process), access, distant::detail::process_base_tag{});
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
	access_token<A, K>::access_token(const K& k) noexcept
		: Base(
			distant::handle<access_token>{
				detail::get_token_impl(k, static_cast<boost::winapi::DWORD_>(A), detail::dispatcher<K>::dispatch{})
			}) {}

	template <access_rights::token A, typename K>
	bool access_token<A, K>::has_privilege(const security::privilege& p) const noexcept
	{
		if (!p) return false;

		boost::winapi::BOOL_ result = false;
		boost::winapi::PRIVILEGE_SET_ set = p;

		boost::winapi::privilege_check(this->get_handle().native_handle(), &set, &result);
		return result;
	}

	template <access_rights::token A, typename K>
	bool access_token<A, K>::set_privilege(const security::privilege& p,
	                                       security::privilege::attributes attribute) noexcept
	{
		if (!p) return false;

		boost::winapi::TOKEN_PRIVILEGES_ temp = p;
		temp.Privileges[0].Attributes = static_cast<boost::winapi::DWORD_>(attribute);

		if (boost::winapi::adjust_token_privilege(this->get_handle().native_handle(), false, &temp, sizeof(temp), nullptr,
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
	template <access_rights::token Access, typename KernelObject>
	access_token<Access, KernelObject>
	get_access_token(const KernelObject& object) noexcept
	{
		return access_token<Access, KernelObject>{object};
	}

	template <typename KernelObject>
	access_token<access_rights::token::adjust_privileges | access_rights::token::query, KernelObject>
	get_access_token(const KernelObject& object) noexcept
	{
		return get_access_token<access_rights::token::adjust_privileges | access_rights::token::query>(object);
	}

	inline access_token<access_rights::token::all_access, process<>>
	get_access_token() noexcept
	{
		using Token = access_token<access_rights::token::all_access, process<>>;
		return std::move(
			reinterpret_cast<Token&>(
				get_access_token(kernel::current_process<>())
			)
		);
	}
} // namespace distant::kernel
