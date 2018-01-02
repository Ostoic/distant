#pragma once

#include <string>
#include <distant\security\luid.hpp>
#include <distant\config.hpp>

#include <distant\support\winapi\privilege.hpp>
#include <distant\support\winapi\token.hpp>

namespace distant::security {

	class privilege
	{
	public:
		enum class attribute : long long int
		{
			enabled = boost::winapi::SE_PRIVILEGE_ENABLED_,
			enabled_by_default = boost::winapi::SE_PRIVILEGE_ENABLED_BY_DEFAULT_,
			removed = boost::winapi::SE_PRIVILEGE_REMOVED_,
			used_for_access = boost::winapi::SE_PRIVILEGE_USED_FOR_ACCESS_,
		};

	public:
		privilege() noexcept = default;

		constexpr explicit privilege(security::luid luid, attribute attrib = attribute::enabled) noexcept;

		explicit privilege(const std::wstring& privilegeName, attribute attrib = attribute::enabled);

		operator boost::winapi::TOKEN_PRIVILEGES_() const noexcept;

		operator boost::winapi::PRIVILEGE_SET_() const noexcept;

		explicit operator bool() const noexcept;

	private:
		attribute m_attribute = attribute::enabled;
		security::luid m_luid;
	};

	// XXX Look into how Windows programmers properly do this
	// Lookup the name of the given privilege (identified by luid)
	std::wstring lookup_name(security::luid luid);

	// Lookup the privilege local UID and attribute given the name.
	privilege lookup_privilege(distant::config::string_view privilege_name);

} // end namespace distant::security

// Implementation
#include <distant\security\detail\privilege.hxx>
