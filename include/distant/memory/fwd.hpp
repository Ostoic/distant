// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/agents/fwd.hpp>
#include <distant/types.hpp>

namespace distant::memory
{
	template <typename Element, typename AddressT, process_rights AccessRights>
	class virtual_ptr;

	template <typename Element, typename AddressT, process_rights AccessRights>
	class virtual_reference;

	template <typename AddressT>
	class protect_guard;

	template <typename AddressT>
	class address;

	template <typename AddressT>
	class page;

	template <typename Signature, typename CallingConv, typename AddressT, process_rights AccessRights>
	class function;
}
