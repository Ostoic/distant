#pragma once

#include <distant\security\luid.hpp>
#include <distant\config.hpp>

#include <Windows.h>

namespace distant::security {

	class privilege
	{
	public:
		enum class attribute
		{
			enabled = SE_PRIVILEGE_ENABLED,
			enabled_by_default = SE_PRIVILEGE_ENABLED_BY_DEFAULT,
			removed = SE_PRIVILEGE_REMOVED,
			used_for_access = SE_PRIVILEGE_USED_FOR_ACCESS,
		};

	public:
		constexpr privilege() noexcept;

		explicit privilege(security::luid luid, attribute attrib = attribute::enabled) noexcept;

		operator TOKEN_PRIVILEGES() const noexcept;

		explicit operator bool() const noexcept;

	private:
		attribute m_attribute = attribute::enabled;
		security::luid m_luid;
		std::size_t m_count = 0;
	};

	// XXX Look into how Windows programmers properly do this
	// Lookup the name of the given privilege (identified by luid)
	distant::config::string lookup_name(security::luid luid);

	// Lookup the privilege local UID and attribute given the name.
	privilege lookup_privilege(distant::config::string_view privilege_name);



} // end namespace distant::security

// Implementation
#include <distant\security\detail\privileges.inl>