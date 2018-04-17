// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/type_traits.hpp>
#include <distant/kernel_objects/type_traits.hpp>
#include <distant/handle.hpp>

#include <distant/security/privilege.hpp>
#include <distant/kernel_objects/process.hpp>

namespace distant
{
	namespace kernel_objects
	{
		// Container-like object for privileges, SIDs, and other security stuff.
		// TODO: Make into a range that enumerates enabled privileges.
		template <access_rights::token Access, typename KernelObject>
		class access_token : public kernel_object
		{
		private: // subtypes
			using object_type = typename kernel_object_traits<KernelObject>::object_type;
			using Base = kernel_object;

		public: // interface
			/// Check if the given privilege is in the access token's list of privileges.
			/// @param p the privilege to test.
			/// @return true if the privilege is enabled for the access token.
			bool has_privilege(const security::privilege& p) const noexcept;

			/// Enable/disable the given privilege in the access token
			/// @param p the privilege to set.
			/// @param attribute enable, remove, or enable by default the privilege.
			/// @return true if the privilege has been successfully set.
			bool set_privilege(const security::privilege& p,
			                   security::privilege::attributes attribute = security::privilege::attributes::enabled) noexcept;

			/// Remove the given privilege from the access token.
			/// @param p the privilege to remove.
			/// @return true if the privilege has been successfully removed.
			bool remove_privilege(const security::privilege& p) noexcept;

		public: // {ctor}
			access_token() noexcept = default;

			explicit access_token(const KernelObject& k) noexcept;

		protected:
			// Expose implementation to other access token types
			template <access_rights::token, class>
			friend class access_token;
		};

		/// @brief Retrieves the access token of the given kernel object.
		/// @return the primary access token of the kernel object is a process, or it returns an impersonation access token if the kernel object is a thread.
		template <access_rights::token Access, typename KernelObject, typename = std::enable_if_t<detail::has_token_access<KernelObject>::value, KernelObject>>
		access_token<Access, KernelObject> 
			get_access_token(const KernelObject&) noexcept;

		/// @brief Retrieves the access token of the given kernel object.
		/// @return the primary access token of the kernel object is a process, or it returns an impersonation access token if the kernel object is a thread.
		template <typename KernelObject, typename = std::enable_if_t<detail::has_token_access<KernelObject>::value, KernelObject>>
		access_token<access_rights::token::adjust_privileges | access_rights::token::query, KernelObject>
			get_access_token(const KernelObject&) noexcept;

		/// @brief Retrieve the access token of the current process.
		/// @return the primary access token of the current process.
		access_token<access_rights::token::all_access, process<>>
			get_access_token() noexcept;
	} // namespace kernel_objects

	using kernel_objects::access_token;
	using kernel_objects::get_access_token;
} // namespace distant

#include <distant/kernel_objects/impl/access_token.hxx>
