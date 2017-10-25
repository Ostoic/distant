#pragma once

#include <distant\utility\type_traits.hpp>
#include <distant\handle.hpp>

#include <distant\security\privileges.hpp>

namespace distant::security 
{
	template <access_rights::token access, typename KernelObject>
	class access_token : public error::gle
	{
	private:
		using object_type = typename object_traits<KernelObject>::object_type;

	public:
		constexpr access_token() = default;
		explicit access_token(const KernelObject&) noexcept;
		explicit access_token(handle<access_token>&&) noexcept;

		// Bivariant move construtible
		template <access_rights::token OtherAccess, typename OtherObject>
		access_token(access_token<OtherAccess, OtherObject>&& other) noexcept;

		// Bivariant move assignable
		template <access_rights::token OtherAccess, typename OtherObject>
		access_token& operator= (access_token<OtherAccess, OtherObject>&& other) noexcept;

		// Not copy constructible
		access_token(const access_token&) = delete;

		// Not copy assignable
		access_token& operator=(const access_token&) = delete;

		bool adjust(security::privilege p) noexcept;

		explicit operator bool() const noexcept;

	public:
		class information;

	protected:
		// Expose implementation to other access token types
		template <access_rights::token, class>
		friend class access_token;

		using expose = distant::detail::attorney::to_handle<access_token>;

		handle<access_token> m_handle;
	};

	template <access_rights::token access, typename KernelObject>
	access_token<access, KernelObject> get_token(const KernelObject&) noexcept;
}

#include <distant\security\detail\access_token.inl>
