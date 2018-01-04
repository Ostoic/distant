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
		enum class attributes : long long int
		{
			enabled = boost::winapi::SE_PRIVILEGE_ENABLED_,
			enabled_by_default = boost::winapi::SE_PRIVILEGE_ENABLED_BY_DEFAULT_,
			removed = boost::winapi::SE_PRIVILEGE_REMOVED_,
			used_for_access = boost::winapi::SE_PRIVILEGE_USED_FOR_ACCESS_,
		};

	public: // {ctor}
		privilege() noexcept = default;

		constexpr explicit privilege(security::luid luid, attributes attrib = attributes::enabled) noexcept;

		explicit privilege(const std::string& privilegeName, attributes attrib = attributes::enabled);

	public: // interface
		operator boost::winapi::TOKEN_PRIVILEGES_() const noexcept;

		operator boost::winapi::PRIVILEGE_SET_() const noexcept;

		explicit operator bool() const noexcept;

	public: // data
		attributes attribute = attributes::enabled;
		security::luid luid;
	};

	// XXX Look into how Windows programmers properly do this
	// Lookup the name of the given privilege (identified by luid)
	std::wstring lookup_name(security::luid luid);

	// Lookup the privilege local UID and attribute given the name.
	privilege lookup_privilege(const std::string& privilegeName);

} // end namespace distant::security

// Implementation
#include <distant\security\impl\privilege.hxx>
