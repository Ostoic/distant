#pragma once

#include <distant\utility\type_traits.hpp>
#include <distant\handle.hpp>

#include <distant\security\privilege.hpp>

namespace distant {
namespace kernel  {

	// Container-like object for privileges, SIDs, and other security stuff.
	// TODO: Make into a range that enumerates enabled privileges.
	template <access_rights::token access, typename KernelObject>
	class access_token : public kernel::object
	{
	private: // subtypes
		using object_type = typename object_traits<KernelObject>::object_type;
		using Base = kernel::object;

	public: // interface
		/// Check if the given privilege is in the access token's list of privileges.
		/// \param p the privilege to test.
		/// \return true if the privilege is enabled for the access token.
		bool has_privilege(const security::privilege& p) const noexcept;

		/// Enable/disable the given privilege in the current access token
		/// \param p the privilege to change
		/// \return true if the privilege has been successfully set.
		bool set_privilege(const security::privilege& p, security::privilege::attributes attribute = security::privilege::attributes::enabled) noexcept;

		bool remove_privilege(const security::privilege& p) noexcept;

	public: // {ctor}
		access_token() noexcept = default;

		explicit access_token(const KernelObject& k) noexcept;

		access_token(access_token&&) noexcept = default;

	protected:
		// Expose implementation to other access token types
		template <access_rights::token, class>
		friend class access_token;
	};

	/// Retrieves the access token of the given kernel object.
	/// \return the primary access token if the kernel object is a process,
	/// or it returns an impersonation access token if the kernel object is a thread.
	template <access_rights::token access, typename KernelObject>
	access_token<access, KernelObject> get_access_token(const KernelObject&) noexcept;

	/// Retrieves the access token of the given kernel object.
	/// \return the primary access token if the kernel object is a process,
	/// or it returns an impersonation access token if the kernel object is a thread.
	template <typename KernelObject>
	access_token<access_rights::token::adjust_privileges, KernelObject> get_access_token(const KernelObject&) noexcept;

} // namespace kernel

using kernel::access_token;
using kernel::get_access_token;

} // namespace distant

#include <distant\kernel\impl\access_token.hxx>
