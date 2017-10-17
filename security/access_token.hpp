#pragma once

#include <distant\handle.hpp>

#include <windows.h>

namespace distant::security 
{
	template <access_rights::token access, typename KernelObject>
	class access_token
	{
	public:
		constexpr access_token() = default;

		explicit access_token(handle<access_token>&&);

		template <access_rights::token OtherAccess, typename OtherObject>

		template <access_rights::token OtherAccess, typename OtherObject>
		access_token& operator= (access_token<OtherAccess, OtherObject>&& other);

		void adjust();

	protected:
		// Expose implementation to other access token types
		template <access_rights::token, class>
		friend class access_token;

		handle<access_token> m_handle;
	};

	template <access_rights::token access, typename KernelObject>
	access_token<access, KernelObject> get_token(const KernelObject&) noexcept;
}

#include <distant\security\detail\access_token.inl>
