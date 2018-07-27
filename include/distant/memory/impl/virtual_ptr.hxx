// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once
#include "../virtual_ptr.hpp"

namespace distant::memory
{
	template <class E, class Ad, process_rights A>
	constexpr virtual_ptr<E, Ad, A>::virtual_ptr(nullptr_t) noexcept
		: virtual_ptr()
	{}

	template <class E, class Ad, process_rights A>
	virtual_ptr<E, Ad, A>::virtual_ptr(process_type& process, address<Ad> address) noexcept
		: process_(&process), address_(address)
	{}

	template <class E, class Ad, process_rights A>
	template <class OtherT, class OtherAddressT, process_rights OtherAccess, class>
	virtual_ptr<E, Ad, A>::virtual_ptr(virtual_ptr<OtherT, OtherAddressT, OtherAccess> pointer) noexcept
		: process_(reinterpret_cast<decltype(process_)>(pointer.process_)), address_(pointer.address_)
	{
		static_assert(require_vm_access_to<OtherT>::value, "[virtual_ptr::{ctor}] Conversion loses const qualifier");
	}

	template <class E, class Ad, process_rights A>
	void virtual_ptr<E, Ad, A>
		::increment() noexcept
	{
		address_ += sizeof(E);
	}

	template <class E, class Ad, process_rights A>
	void virtual_ptr<E, Ad, A>
		::decrement() noexcept
	{
		address_ -= sizeof(E);
	}

	template <class E, class Ad, process_rights A>
	typename virtual_ptr<E, Ad, A>::reference
	virtual_ptr<E, Ad, A>
		::dereference() const noexcept
	{
		return reference(*this);
	}

	template <class E, class Ad, process_rights A>
	void virtual_ptr<E, Ad, A>
		::advance(const difference_type n) noexcept
	{
		address_ += sizeof(E) * n;
	}

	template <class E, class Ad, process_rights A>
	template <class OtherT, class OtherAddressT, process_rights OtherAccess>
	bool virtual_ptr<E, Ad, A>
		::equal(virtual_ptr<OtherT, OtherAddressT, OtherAccess> other) const noexcept
	{
		static_assert(require_vm_access_to<OtherT>::value, "[virtual_ptr::equal] Conversion loses const qualifier");
		return address_ == other.address_ && *process_ == *other.process_;
	}

	template <class E, class Ad, process_rights A>
	template <class OtherT, class OtherAddressT, process_rights OtherAccess>
	typename virtual_ptr<E, Ad, A>::difference_type virtual_ptr<E, Ad, A>
		::distance_to(virtual_ptr<OtherT, OtherAddressT, OtherAccess> other) const noexcept
	{
		static_assert(require_vm_access_to<OtherT>::value, "[virtual_ptr::distance_to] Conversion loses const qualifier");
		return static_cast<Ad>(other.address_ - address_);
	}

	template <class E, class Ad, process_rights A>
	address<Ad> virtual_ptr<E, Ad, A>
		::get() const noexcept
	{
		return address_;
	}

} // namespace distant::memory
