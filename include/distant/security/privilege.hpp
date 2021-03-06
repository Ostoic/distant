// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <string>
#include <distant/security/luid.hpp>
#include <distant/config.hpp>

#include <distant/support/winapi/privilege.hpp>
#include <distant/support/winapi/token.hpp>

namespace distant::security
{
	/// @brief A [privilege](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379306(v=vs.85).aspx) is the right of an account, such as a user or group account, to perform various system-related operations on the local computer, such as shutting down the system, loading device drivers, or changing the system time.
	/// a \a security::privilege allows one to modify a \a kernel_object's \a access_token to constrain or relax permissions for the given \a kernel_object.
	class privilege
	{
	public:
		enum class attributes : long long int
		{
			enabled = boost::winapi::SE_PRIVILEGE_ENABLED_,
			enabled_by_default = boost::winapi::SE_PRIVILEGE_ENABLED_BY_DEFAULT_,
			removed = boost::winapi::SE_PRIVILEGE_REMOVED_,
			used_for_access = boost::winapi::SE_PRIVILEGE_USED_FOR_ACCESS_,
		};

	public: // {ctor}
		/// @brief Constructs an invalid privilege object.
		constexpr privilege() noexcept;

		/// @brief Constexpr constructor allowing the privilege's data to be loaded on demand.
		explicit constexpr privilege(const wchar_t* privilege_name) noexcept;

	public: // interface
		/// @brief Implicility convertible to the TOKEN_PRIVILEGES_ struct.
		/// @return an instance of TOKEN_PRIVILEGES_ filled with the current privilege's data.
		operator boost::winapi::TOKEN_PRIVILEGES_() const noexcept;

		/// @brief Implicitly convertible to a PRIVILEGE_SET_ object.
		/// @return an instance of PRIVILEGE_SET_ filled with the current privilege's data.
		operator boost::winapi::PRIVILEGE_SET_() const noexcept;

		/// @brief Get the Locally Unique Identifier associated with the given privilege.
		/// @return the luid returned by the operating system for the privilege.
		security::luid luid() const noexcept;

		explicit operator bool() const noexcept;

	private:
		void update_luid() const noexcept;

		const wchar_t* name_;
		mutable security::luid luid_;
	};

	/// Todo: Look into how Windows programmers properly do this
	/// @brief Lookup the name of the given privilege (identified by luid)
	std::wstring lookup_name(security::luid luid);

	/// @brief Lookup the privilege local UID and attribute given the name.
	privilege lookup_privilege(const std::wstring& privilege_name);

} // end namespace distant::security

// Implementation
#include <distant/security/impl/privilege.hxx>
