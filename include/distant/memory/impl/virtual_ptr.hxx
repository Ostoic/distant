// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once
#include "../virtual_ptr.hpp"

namespace distant::memory
{
	template <typename E, typename Ad>
	virtual_ptr<E, Ad>::virtual_ptr(nullptr_t) noexcept
		: virtual_ptr()
	{}

	template <typename E, typename Ad>
	virtual_ptr<E, Ad>::virtual_ptr(const kernel_objects::process<vm_access>& process, address<Ad> address) noexcept
		: process_(reinterpret_cast<decltype(process_)>(&process)), address_(address)
	{}

	template <typename E, typename Ad>
	template <typename OtherT, typename OtherAddressT, typename>
	virtual_ptr<E, Ad>::virtual_ptr(virtual_ptr<OtherT, OtherAddressT> pointer) noexcept
		: process_(reinterpret_cast<decltype(process_)>(pointer.process_)), address_(pointer.address_)
	{}

	template <typename E, typename Ad>
	void virtual_ptr<E, Ad>::increment() noexcept
	{
		address_ += sizeof(E);
	}

	template <typename E, typename Ad>
	void virtual_ptr<E, Ad>::decrement() noexcept
	{
		address_ -= sizeof(E);
	}

	template <typename E, typename Ad>
	typename virtual_ptr<E, Ad>::reference
	virtual_ptr<E, Ad>::dereference() const noexcept
	{
		return reference(*this);
	}

	template <typename E, typename Ad>
	void virtual_ptr<E, Ad>::advance(const difference_type n) noexcept
	{
		address_ += sizeof(E) * n;
	}

	template <typename E, typename Ad>
	template <typename OtherT, typename OtherAddressT, typename>
	bool virtual_ptr<E, Ad>::equal(virtual_ptr<OtherT, OtherAddressT> other) const noexcept
	{
		return address_ == other.address_ && *process_ == *other.process_;
	}

	template <typename E, typename Ad>
	template <typename OtherT, typename OtherAddressT, typename>
	typename virtual_ptr<E, Ad>::difference_type 
	virtual_ptr<E, Ad>::distance_to(virtual_ptr<OtherT, OtherAddressT> other) const noexcept
	{
		return other.address_ - address_;
	}

	template <typename E, typename Ad>
	address<Ad> virtual_ptr<E, Ad>::get() const noexcept
	{
		return address_;
	}

} // namespace distant::memory
