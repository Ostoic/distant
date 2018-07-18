// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/memory/virtual_ptr.hpp>

namespace distant
{
	namespace memory
	{
		template <typename Element, typename AddressT, process_rights AccessRights>
		class virtual_reference
		{
		public:
			using pointer      = typename std::pointer_traits<virtual_ptr<Element, AddressT, AccessRights>>::pointer;
			using element_type = typename std::pointer_traits<virtual_ptr<Element, AddressT, AccessRights>>::element_type;

			static constexpr auto vm_access = virtual_traits<virtual_reference>::vm_access;
			using process_type = typename pointer::process_type;

			template <typename T>
			using require_vm_access_to = std::enable_if_t<detail::required_vm_access<T>::value >= vm_access>;

		public:
			explicit virtual_reference(pointer ptr) noexcept;

			template <
				typename OtherElement,
				typename OtherAddressT,
				process_rights OtherAccess
			>
			virtual_reference(virtual_reference<OtherElement, OtherAddressT, OtherAccess> other) noexcept;

			template <
				typename OtherElement,
				typename OtherAddressT,
				process_rights OtherAccess
			>
			virtual_reference& operator=(virtual_reference<OtherElement, OtherAddressT, OtherAccess> other) noexcept;

			template <
				typename Value,
				typename = std::enable_if_t<
					std::is_convertible<Value, element_type>::value &&
					!std::is_const<Value>::value
				>
			>
			virtual_reference& operator=(const Value& x);

			pointer operator&() const noexcept;

			operator element_type() const;

			// can we somehow do noexcept?
			void swap(virtual_reference& other);

			// These operators can't rely on an implicitly converted implementation since they act on the class and return another instance of the class.
			virtual_reference& operator++();

			element_type operator++(int);

			virtual_reference& operator--();

			element_type operator--(int);

			virtual_reference& operator+=(const element_type& rhs);

			virtual_reference& operator-=(const element_type& rhs);

			virtual_reference& operator*=(const element_type& rhs);

			virtual_reference& operator/=(const element_type& rhs);

			virtual_reference& operator%=(const element_type& rhs);

			virtual_reference& operator<<=(const element_type& rhs);

			virtual_reference& operator>>=(const element_type& rhs);

			virtual_reference& operator&=(const element_type& rhs);

			virtual_reference& operator|=(const element_type& rhs);

			virtual_reference& operator^=(const element_type& rhs);

			process_type& process() const noexcept { return ptr_.process(); }
			process_type& process()		  noexcept { return ptr_.process(); }

		private:
			template <typename E, typename Ad, process_rights R>
			friend class virtual_reference;

			template <typename E, typename Ad, process_rights R>
			friend class virtual_ptr;

			pointer ptr_;

		}; // class virtual_reference

		template <typename Element, typename AddressT, process_rights Access>
		auto make_virtual_reference(process<Access>& process, const address<AddressT> address) noexcept
		{ return *make_virtual_ptr<Element, AddressT>(process, address);}

		template <typename Element, typename AddressT, process_rights Access>
		auto make_virtual_reference(const process<Access>& process, const address<AddressT> address) noexcept
		{ return *make_virtual_ptr<Element, AddressT>(process, address);}

		template <typename Element, process_rights Access>
		auto make_virtual_reference(process<Access>& process, const address<dword> address) noexcept
		{ return *make_virtual_ptr<Element, dword>(process, address);}

		template <typename Element, process_rights Access>
		auto make_virtual_reference(const process<Access>& process, const address<dword> address) noexcept
		{ return *make_virtual_ptr<Element, dword>(process, address);}

		template <typename Element, typename AddressT, process_rights Access>
		struct virtual_traits<virtual_reference<Element, AddressT, Access>>
			 : virtual_traits<virtual_ptr<Element, AddressT, Access>>
		{};

	} // namespace memory

	using memory::virtual_reference;
	using memory::make_virtual_reference;

} // namespace distant


#include "impl/virtual_reference.hxx"
