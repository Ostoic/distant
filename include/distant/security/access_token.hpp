// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/config.hpp>
#include <distant/type_traits.hpp>
#include <distant/scoped_handle.hpp>

#include <distant/security/privilege.hpp>
#include <distant/agents/process.hpp>
#include <distant/concepts/handleable.hpp>
#include <distant/concepts/equality_comparable.hpp>

#include <distant/security/detail/token_access.hpp>

namespace distant::security
{
	// Range-like object for privileges, SIDs, and other security stuff.
	// TODO: Make into a range that enumerates enabled privileges.
	template <token_rights Access, class Agent>
	class access_token
		: concepts::handleable<access_token<Access, Agent>>
		, boost::equality_comparable<access_token<Access, Agent>>
	{
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

		explicit access_token(const Agent& k) noexcept;

		friend bool operator==(const access_token& lhs, const access_token& rhs) noexcept
		{ return lhs.handle_ == rhs.handle_; }

	protected:
		template <token_rights, class> friend class access_token;
		template <class> friend struct concepts::handleable;

		kernel_handle handle_;
	};

	/// @brief Retrieves the access token of the given kernel object.
	/// @return the primary access token of the kernel object is a process, or it returns an impersonation access token if the kernel object is a thread.
	template <token_rights Access, class Agent>
	access_token<Access, Agent>
		primary_access_token(const Agent&) noexcept;

	/// @brief Retrieves the access token of the given kernel object.
	/// @return the primary access token of the kernel object is a process, or it returns an impersonation access token if the kernel object is a thread.
	template <class Agent>
	access_token<token_rights::adjust_privileges | token_rights::query, Agent>
		primary_access_token(const Agent&) noexcept;

	/// @brief Retrieve the access token of the current process.
	/// @return the primary access token of the current process.
	access_token<token_rights::all_access, process<>>
		primary_access_token() noexcept;

} // namespace distant::security

namespace distant 
{
	using security::access_token;
	using security::primary_access_token;
}

#include "impl/access_token.hxx"
