#pragma once
#include <distant\security\privilege.hpp>

#include <distant\support\winapi\privilege.hpp>
#include <distant\error\windows_error.hpp>

#include <string>
#include <string_view>

namespace distant::security {

//class privilege
//public:
	inline constexpr privilege::privilege(security::luid luid, attributes attrib) noexcept
		: attribute(attrib), luid(luid) {}

	inline privilege::privilege(const std::string& privilegeName, attributes attrib)
	{
		security::luid luid;
		if (!boost::winapi::lookup_privilege_value(NULL, privilegeName.c_str(), &luid))
			throw std::system_error(error::windows_error_code(error::gle()), "Invalid privilege value");

		this->luid = luid;
	}

	inline privilege::operator boost::winapi::TOKEN_PRIVILEGES_() const noexcept
	{
		using boost::winapi::DWORD_;

		boost::winapi::TOKEN_PRIVILEGES_ temp;

		temp.PrivilegeCount = 1;
		temp.Privileges[0].Attributes = static_cast<DWORD_>(this->attribute);
		temp.Privileges[0].Luid = this->luid;
		return temp;
	}

	inline privilege::operator boost::winapi::PRIVILEGE_SET_() const noexcept
	{
		using boost::winapi::DWORD_;

		boost::winapi::PRIVILEGE_SET_ temp;
		temp.Privilege[0].Luid = this->luid;
		temp.Privilege[0].Attributes = static_cast<DWORD_>(this->attribute);
		temp.PrivilegeCount = 1;
		temp.Control = boost::winapi::PRIVILEGE_SET_ALL_NECESSARY_;
		return temp;
	}

	inline privilege::operator bool() const noexcept
	{
		return this->luid.LowPart != 0 || this->luid.HighPart != 0;
	}

//free:
	// XXX Look into how Windows programmers properly do this
	// Lookup the name of the given privilege (identified by luid)
	inline std::wstring lookup_name(security::luid luid)
	{
		boost::winapi::DWORD_ size = 100;
		wchar_t buffer[100];

		if (!boost::winapi::lookup_privilege_name(NULL, &luid, buffer, &size))
			throw std::system_error(error::windows_error_code(error::gle()), "Privilege name lookup failed.");

		return std::move(buffer);
	}

	// XXX Input domain should be restricted to SE_XXX_NAME macro types
	// Lookup the privilege local UID and attribute given the name.
	inline privilege lookup_privilege(const std::string& privilegeName)
	{
		security::luid luid;
		if (!boost::winapi::lookup_privilege_value(NULL, privilegeName.c_str(), &luid))
			return {};

		return privilege(luid);
	}

} // end namespace distant::security