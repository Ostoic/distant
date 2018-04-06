// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <string>
#include <distant/security/luid.hpp>
#include <distant/config.hpp>

#include <distant/support/winapi/privilege.hpp>
#include <distant/support/winapi/token.hpp>

namespace distant::security {

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
		/// Constructs an invalid privilege object.
		constexpr privilege() noexcept;

		/// Constexpr constructor allowing the privilege's data to be loaded on demand.
		explicit constexpr privilege(const wchar_t* privilegeName) noexcept;

	public: // interface
		/// Implicility convertible to the TOKEN_PRIVILEGES_ struct.
		/// @return an instance of TOKEN_PRIVILEGES_ filled with the current privilege's data.
		operator boost::winapi::TOKEN_PRIVILEGES_() const noexcept;

		/// Implicitly convertible to a PRIVILEGE_SET_ object.
		/// @return an instance of PRIVILEGE_SET_ filled with the current privilege's data.
		operator boost::winapi::PRIVILEGE_SET_() const noexcept;

		/// Get the Locally Unique Identifier associated with the given privilege.
		/// @return the luid returned by the operating system for the privilege.
		security::luid luid() const noexcept;

		explicit operator bool() const noexcept;

	private:
		const wchar_t* name_;
		mutable security::luid luid_;
	};

	// XXX Look into how Windows programmers properly do this
	// Lookup the name of the given privilege (identified by luid)
	std::wstring lookup_name(security::luid luid);

	// Lookup the privilege local UID and attribute given the name.
	privilege lookup_privilege(const std::wstring& privilegeName);

} // end namespace distant::security

// Implementation
#include <distant/security/impl/privilege.hxx>
