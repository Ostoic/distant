#pragma once
#include <distant\security\privilege.hpp>

#include <distant\support\winapi\privilege.hpp>
#include <distant\error\windows_error.hpp>

#include <string>
#include <string_view>

namespace distant::security {

//class privilege
//public:
	inline privilege::privilege(security::luid luid, attribute attrib) noexcept
		: m_attribute(attrib), m_luid(luid) {}

	inline privilege::privilege(const std::string& privilegeName, attribute attrib)
	{
		security::luid luid;
		if (!boost::winapi::lookup_privilege_value(NULL, privilegeName.c_str(), &luid))
			throw std::system_error(error::windows_error(error::gle()), "Invalid privilege value");

		m_luid = luid;
	}

	inline privilege::operator boost::winapi::TOKEN_PRIVILEGES_() const noexcept
	{
		using boost::winapi::DWORD_;

		boost::winapi::TOKEN_PRIVILEGES_ temp;

		temp.PrivilegeCount = 1;
		temp.Privileges[0].Attributes = static_cast<DWORD_>(m_attribute);
		temp.Privileges[0].Luid = m_luid;
		return temp;
	}

	inline privilege::operator boost::winapi::PRIVILEGE_SET_() const noexcept
	{
		using boost::winapi::DWORD_;

		boost::winapi::PRIVILEGE_SET_ temp;
		temp.Privilege[0].Luid = m_luid;
		temp.Privilege[0].Attributes = static_cast<DWORD_>(m_attribute);
		temp.PrivilegeCount = 1;
		temp.Control = PRIVILEGE_SET_ALL_NECESSARY;
		return temp;
	}

	inline privilege::operator bool() const noexcept
	{
		return m_luid.LowPart != 0 || m_luid.HighPart != 0;
	}

//free:
	// XXX Look into how Windows programmers properly do this
	// Lookup the name of the given privilege (identified by luid)
	inline std::string lookup_name(security::luid luid)
	{
		boost::winapi::DWORD_ size = 100;
		char buffer[100];

		if (!boost::winapi::lookup_privilege_name(NULL, &luid, buffer, &size))
			throw std::system_error(error::windows_error(error::gle()), "Privilege name lookup failed.");

		return std::move(buffer);
	}

	// XXX Input domain should be restricted to SE_XXX_NAME macro types
	// Lookup the privilege local UID and attribute given the name.
	inline privilege lookup_privilege(const std::string& privilege_name)
	{
		security::luid luid;
		if (!boost::winapi::lookup_privilege_value(NULL, privilege_name.c_str(), &luid))
			return {};

		return privilege(luid);
	}

} // end namespace distant::security