#pragma once

#include <distant/kernel/fwd.hpp>
#include <distant/types.hpp>

namespace distant::memory
{
	template <typename Element, process_rights Access, typename AddressT>
	class virtual_ptr;

	template <typename Element, process_rights Access, typename AddressT>
	class virtual_reference;

}
