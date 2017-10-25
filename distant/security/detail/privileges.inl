#pragma once

#include <distant\security\privileges.hpp>

namespace distant::security {

//class privilege
//public:
	inline constexpr privilege::privilege() noexcept
		: m_count(0) {}

	inline privilege::privilege(security::luid luid, attribute attrib) noexcept
		: m_attribute(attrib), m_luid(luid), m_count(1) {}

	inline privilege::operator TOKEN_PRIVILEGES() const noexcept
	{
		TOKEN_PRIVILEGES temp;
		temp.PrivilegeCount = m_count;
		temp.Privileges[0].Attributes = static_cast<DWORD>(m_attribute);
		temp.Privileges[0].Luid = m_luid;
		return temp;
	}

	inline privilege::operator bool() const noexcept
	{
		return m_count > 0;
	}

//free:
	// XXX Look into how Windows programmers properly do this
	// Lookup the name of the given privilege (identified by luid)
	inline distant::config::string lookup_name(security::luid luid)
	{
		DWORD size = 100;
		distant::config::string_elem buffer[100];

		if (!::LookupPrivilegeName(NULL, &luid, buffer, &size))
			throw std::runtime_error("Unable to retrieve privilege name");

		return distant::config::string(std::move(buffer));
	}

	// XXX Input domain should be restricted to SE_XXX_NAME macro types
	// Lookup the privilege local UID and attribute given the name.
	inline privilege lookup_privilege(distant::config::string_view privilege_name)
	{
		security::luid luid;
		if (!::LookupPrivilegeValue(NULL, privilege_name.data(), &luid))
			return {};

		return privilege(luid);
	}

} // end namespace distant::security