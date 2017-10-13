#pragma once

#include <distant\windows\handle.hpp>
#include <distant\windows\access_rights.hpp>

#include <windows.h>

namespace distant::windows::security 
{
	// 
	template <access_rights::token access, typename KernelObject>
	class access_token
	{
	public:
		constexpr access_token() = default;

		explicit access_token(windows::handle<access_token>&&);

		template <access_rights::token OtherAccess, typename OtherObject>
		explicit access_token(const access_token<OtherAccess, OtherObject>&& other);

		template <access_rights::token OtherAccess, typename OtherObject>
		access_token& operator= (access_token<OtherAccess, OtherObject>&& other);

		void adjust();

	protected:
		// Expose implementation to other access token types
		template <access_rights::token, class>
		friend class access_token;

		windows::handle<access_token> m_handle;
	};

	template <access_rights::token access, typename KernelObject>
	access_token<access, KernelObject> get_token(const KernelObject&) noexcept;

}

#include <distant\windows\security\detail\access_token.inl>