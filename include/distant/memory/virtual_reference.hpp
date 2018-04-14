// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/memory/virtual_ptr.hpp>

namespace distant
{
	namespace memory
	{
		template <typename Element, typename AddressT = dword>
		class virtual_reference
		{
		public:
			using pointer    = typename std::pointer_traits<virtual_ptr<Element, AddressT>>::pointer;
			using value_type = typename std::pointer_traits<virtual_ptr<Element, AddressT>>::element_type;

			static constexpr auto vm_access = virtual_traits<virtual_reference>::vm_access;

			template <typename T>
			using require_vm_access_to = std::enable_if_t<detail::required_vm_access<T>::value >= vm_access>;

		public:
			explicit virtual_reference(pointer ptr) noexcept;
			
			template <
				typename OtherElement,
				typename OtherAddressT,
				typename = require_vm_access_to<OtherElement>
			>
			virtual_reference(virtual_reference<OtherElement, OtherAddressT> other) noexcept;

			template <
				typename OtherElement,
				typename OtherAddressT,
				typename = require_vm_access_to<OtherElement>
			>
			virtual_reference& operator=(virtual_reference<OtherElement, OtherAddressT> other) noexcept;

			template <
				typename Value, 
				typename = std::enable_if_t<
					std::is_convertible<Value, value_type>::value && 
					!std::is_const<Value>::value
				>
			>
			virtual_reference& operator=(const Value& x);

			pointer operator&() const noexcept;
			
			operator value_type() const;

			void swap(virtual_reference& other);

			// These operators can't rely on an implicitly converted implementation since they act on the class and return another instance of the class.
			virtual_reference& operator++();

			value_type operator++(int);

			virtual_reference& operator--();

			value_type operator--(int);

			virtual_reference& operator+=(const value_type& rhs);

			virtual_reference& operator-=(const value_type& rhs);

			virtual_reference& operator*=(const value_type& rhs);

			virtual_reference& operator/=(const value_type& rhs);

			virtual_reference& operator%=(const value_type& rhs);

			virtual_reference& operator<<=(const value_type& rhs);

			virtual_reference& operator>>=(const value_type& rhs);

			virtual_reference& operator&=(const value_type& rhs);

			virtual_reference& operator|=(const value_type& rhs);

			virtual_reference& operator^=(const value_type& rhs);

		private:
			template <typename E, typename Ad>
			friend class virtual_reference;

			template <typename E, typename Ad>
			friend class virtual_ptr;

			pointer ptr_;

		}; // class virtual_reference

		template <typename Element, typename AddressT, process_rights Access,
			typename = std::enable_if_t<Access >= detail::required_vm_access<Element>::value>
		>
		auto make_virtual_reference(const process<Access>& process, const address<AddressT> address) noexcept
		{ return *make_virtual_ptr<Element, AddressT>(process, address);}

		template <typename Element, process_rights Access,
			typename = std::enable_if_t<Access >= detail::required_vm_access<Element>::value>
		>
		auto make_virtual_reference(const process<Access>& process, const address<dword> address) noexcept
		{ return *make_virtual_ptr<Element, dword>(process, address);}

		template <typename Element, typename AddressT>
		struct virtual_traits<virtual_reference<Element, AddressT>>
			 : virtual_traits<virtual_ptr<Element, AddressT>>
		{};

	} // namespace memory

	using memory::virtual_reference;
	using memory::make_virtual_reference;

} // namespace distant


#include "impl/virtual_reference.hxx"
