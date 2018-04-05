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
			using require_vm_access_to = std::enable_if_t<detail::required_vm_access<T>::value >= vm_access, T>;

		public:
			// Todo: Fix const corrrectness of casting between nonconst -> const, etc
			/*template <
				typename OtherElement, 
				typename = std::enable_if_t<
					std::is_const<OtherElement>
				>
			>*/
			explicit virtual_reference(pointer ptr);
			
			template <
				typename OtherElement,
				typename OtherAddressT,
				typename = require_vm_access_to<OtherElement>
			>
			virtual_reference(virtual_reference<OtherElement, OtherAddressT> other);

			template <
				typename OtherElement,
				typename OtherAddressT,
				typename = require_vm_access_to<OtherElement>
			>
			virtual_reference& operator=(virtual_reference<OtherElement, OtherAddressT> other);

			template <typename Value, typename = std::enable_if_t<
				!std::is_const<Value>::value && std::is_convertible<Value, value_type>::value>
			>
			virtual_reference& operator=(const Value& x);

			pointer operator&() const;
			
			operator value_type() const;

			void swap(virtual_reference& other);

			// These operators can't rely on an implicitly converted implementation since they act on the class and return another instance of the class.
			virtual_reference& operator++();

			value_type operator++(int);

			virtual_reference& operator+=(const value_type& rhs);

			virtual_reference& operator--();

			value_type operator--(int);

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

			const pointer ptr_;

		}; // class virtual_reference

		template <typename Element, typename AddressT, process_rights Access,
			typename = std::enable_if_t<Access >= detail::required_vm_access<Element>::value>
		>
		auto make_virtual_reference(const process<Access>& process, const address<AddressT> address = nullptr)
		{ return *make_virtual_ptr<Element, AddressT>(process, address);}

		template <typename Element, process_rights Access,
			typename = std::enable_if_t<Access >= detail::required_vm_access<Element>::value>
		>
		auto make_virtual_reference(const process<Access>& process, const address<dword> address = nullptr)
		{ return *make_virtual_ptr<Element>(process, address);}

		template <typename Element, typename AddressT>
		struct virtual_traits<virtual_reference<Element, AddressT>>
			 : virtual_traits<virtual_ptr<Element, AddressT>>
		{};

	} // namespace memory

	/*template <typename Element, typename AddressT = dword>
	using virtual_reference = memory::virtual_reference<Element, vm_rw_op, AddressT>;

	template <typename Element, typename AddressT = dword>
	using const_virtual_reference = memory::virtual_reference<Element, vm_read, AddressT>;*/

	using memory::virtual_reference;
	using memory::make_virtual_reference;

} // namespace distant


#include "impl/virtual_reference.hxx"
