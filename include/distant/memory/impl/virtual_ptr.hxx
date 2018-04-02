#pragma once
#include "../virtual_ptr.hpp"

namespace distant::memory
{
	template <typename E, process_rights A, typename Ad>
	virtual_ptr<E, A, Ad>::virtual_ptr(const kernel::process<A>& process, address<Ad> address)
		: process_(process), address_(address)
	{}

	template <typename E, process_rights A, typename Ad>
	virtual_ptr<E, A, Ad>::virtual_ptr()
		: process_(nullptr), address_(nullptr) 
	{}

	template <typename E, process_rights A, typename Ad>
	template <typename OtherT, process_rights OtherAccess, typename OtherAddressT>
	virtual_ptr<E, A, Ad>::virtual_ptr(const virtual_ptr<OtherT, OtherAccess, OtherAddressT>& pointer)
		: process_(pointer.process_), address_(pointer.address_)
	{}

} // namespace distant::memory
