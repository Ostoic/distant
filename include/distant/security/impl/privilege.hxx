#pragma once
#include <distant\security\privilege.hpp>

#include <distant\support\winapi\privilege.hpp>
#include <distant\error\windows_error.hpp>

#include <string>

namespace distant::security {

//class privilege
//public:
	inline constexpr privilege::privilege() noexcept
		: privilege(nullptr) {}

	inline constexpr privilege::privilege(const wchar_t* privilegeName) noexcept
		: name_(privilegeName)
		, luid_({ 0 }) {}

	inline privilege::operator boost::winapi::TOKEN_PRIVILEGES_() const noexcept
	{
		using boost::winapi::DWORD_;

		boost::winapi::TOKEN_PRIVILEGES_ temp;

		temp.PrivilegeCount = 1;
		temp.Privileges[0].Attributes = static_cast<DWORD_>(attributes::enabled);
		temp.Privileges[0].Luid = this->luid();
		return temp;
	}

	inline privilege::operator boost::winapi::PRIVILEGE_SET_() const noexcept
	{
		using boost::winapi::DWORD_;

		boost::winapi::PRIVILEGE_SET_ temp;
		temp.Privilege[0].Luid = this->luid();
		temp.Privilege[0].Attributes = static_cast<DWORD_>(attributes::enabled);
		temp.PrivilegeCount = 1;
		temp.Control = boost::winapi::PRIVILEGE_SET_ALL_NECESSARY_;
		return temp;
	}

	inline security::luid privilege::luid() const noexcept
	{
		if (this->luid_.LowPart == 0 && this->luid_.HighPart == 0)
		{
			// Lookup LUID given the privilege name.
			if (!boost::winapi::lookup_privilege_value(nullptr, this->name_, &this->luid_))
			{
				// Failed, set invalid luid.
				this->luid_.LowPart = 0;
				this->luid_.HighPart = 0;
			}
		}

		return this->luid_;
	}

	inline privilege::operator bool() const noexcept
	{
		this->luid();
		return this->luid_.LowPart != 0 || this->luid_.HighPart != 0;
	}

//free:
	// XXX Look into how Windows programmers properly do this
	// Lookup the name of the given privilege (identified by luid)
	inline std::wstring lookup_name(security::luid luid)
	{
		boost::winapi::DWORD_ size = 100;
		wchar_t buffer[100];

		if (!boost::winapi::lookup_privilege_name(nullptr, &luid, buffer, &size))
			throw std::system_error(distant::last_error(), "Privilege name lookup failed.");

		return std::move(buffer);
	}

	// XXX Input domain should be restricted to SE_XXX_NAME macro types
	// Lookup the privilege local UID and attribute given the name.
	inline privilege lookup_privilege(const std::wstring& privilegeName)
	{
		return privilege{privilegeName.c_str()};
	}

} // end namespace distant::security