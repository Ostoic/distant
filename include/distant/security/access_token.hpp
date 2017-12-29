#pragma once

#include <distant\utility\type_traits.hpp>
#include <distant\handle.hpp>

#include <distant\security\privilege.hpp>

namespace distant::security 
{
	template <access_rights::token access, typename KernelObject>
	class access_token : public error::windows_error
	{
	private:
		using object_type = typename object_traits<KernelObject>::object_type;

	public:
		constexpr access_token() = default;

		explicit access_token(const KernelObject& k);

		access_token(access_token<access, KernelObject>&&) noexcept = default;

			// Move construtible
		/// Move constructs
		/// \param other the other access token to move from
		template <access_rights::token OtherAccess, typename OtherObject>
		access_token(access_token<OtherAccess, OtherObject>&& other) noexcept;

		// Move assignable
		template <access_rights::token OtherAccess, typename OtherObject>
		access_token& operator=(access_token<OtherAccess, OtherObject>&& other) noexcept = default;

		bool check_privilege(const security::privilege& p) const noexcept;

		/// Enable/disable the given privilege in the current access token
		/// \param p the privilege to change
		void adjust_privilege(const security::privilege& p);

		explicit operator bool() const noexcept;

		// Not copy constructible
		access_token(const access_token&) = delete;

		// Not copy assignable
		access_token& operator=(const access_token&) = delete;

	protected:
		// Expose implementation to other access token types
		template <access_rights::token, class>
		friend class access_token;

		using expose = distant::detail::attorney::to_handle<access_token>;

		handle<access_token> m_handle;
		mutable error::windows_error m_last_error;
	};

	template <access_rights::token access, typename KernelObject>
	access_token<access, KernelObject> get_access_token(const KernelObject&);
}

#include <distant\security\detail\access_token.hxx>
