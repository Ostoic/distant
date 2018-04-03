#pragma once
#include "../virtual_ptr.hpp"

namespace distant::memory
{
	template <typename E, process_rights A, typename Ad>
	virtual_ptr<E, A, Ad>::virtual_ptr(const kernel::process<A>& process, address<Ad> address) noexcept
		: process_(&process), address_(address)
	{}

	template <typename E, process_rights A, typename Ad>
	virtual_ptr<E, A, Ad>::virtual_ptr() noexcept
		: process_(nullptr), address_(nullptr) 
	{}

	template <typename E, process_rights A, typename Ad>
	template <typename OtherT, process_rights OtherAccess, typename OtherAddressT>
	virtual_ptr<E, A, Ad>::virtual_ptr(const virtual_ptr<OtherT, OtherAccess, OtherAddressT>& pointer) noexcept
		: process_(pointer.process_), address_(pointer.address_)
	{}

	template <typename E, process_rights A, typename Ad>
	void virtual_ptr<E, A, Ad>::increment() noexcept
	{
		++this->address_;
	}

	template <typename E, process_rights A, typename Ad>
	void virtual_ptr<E, A, Ad>::decrement() noexcept
	{
		--this->address_;
	}

	template <typename E, process_rights A, typename Ad>
	typename virtual_ptr<E, A, Ad>::const_reference
	virtual_ptr<E, A, Ad>::dereference() const
	{
		return const_reference(*this);
	}

	template <typename E, process_rights A, typename Ad>
	typename virtual_ptr<E, A, Ad>::reference
	virtual_ptr<E, A, Ad>::dereference() 
	{
		return reference(*this);
	}

	template <typename E, process_rights A, typename Ad>
	void virtual_ptr<E, A, Ad>::advance(const int n) noexcept
	{
		this->address_ += n;
	}

	template <typename E, process_rights A, typename Ad>
	template <typename OtherT, process_rights OtherAccess, typename OtherAddressT>
	bool virtual_ptr<E, A, Ad>::equal(const virtual_ptr<OtherT, OtherAccess, OtherAddressT>& other) const noexcept
	{
		return this->address_ == other.address_ && this->process_ == other.process_;
	}

	template <typename E, process_rights A, typename Ad>
	template <typename OtherT, process_rights OtherAccess, typename OtherAddressT>
	typename virtual_ptr<E, A, Ad>::difference_type 
	virtual_ptr<E, A, Ad>::distance_to(const virtual_ptr<OtherT, OtherAccess, OtherAddressT>& other) const noexcept
	{
		return other.address_ - this->address_;
	}

	template <typename E, process_rights A, typename Ad>
	address<Ad> virtual_ptr<E, A, Ad>::get() const noexcept
	{
		return this->address_;
	}

} // namespace distant::memory
