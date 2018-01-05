#pragma once

#include <distant\utility\type_traits.hpp>
#include <distant\handle.hpp>

#include <distant\security\privilege.hpp>

namespace distant::security 
{
	template <access_rights::token access, typename KernelObject>
	class access_token : public utility::boolean_validator<access_token<access, KernelObject>>
	{
	private:
		using object_type = typename object_traits<KernelObject>::object_type;

	public:
		constexpr access_token() = default;

		explicit access_token(const KernelObject& k) noexcept;

		access_token(access_token&&) noexcept = default;

		bool is_enabled(const security::privilege& p) const noexcept;

		/// Enable/disable the given privilege in the current access token
		/// \param p the privilege to change
		void set_privilege(const security::privilege& p);

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
		mutable error::windows_error_code m_last_error;
	};

	template <access_rights::token access, typename KernelObject>
	access_token<access, KernelObject> get_access_token(const KernelObject&);
}

#include <distant\security\impl\access_token.hxx>
