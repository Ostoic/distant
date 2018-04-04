#pragma once

#include <distant/memory/virtual_ptr.hpp>

namespace distant
{
	namespace memory
	{
		template <typename Element, process_rights Access, typename AddressT = dword>
		class virtual_reference
		{
		public:
			using pointer    = typename std::pointer_traits<virtual_ptr<Element, Access, AddressT>>::pointer;
			using value_type = typename std::pointer_traits<virtual_ptr<Element, Access, AddressT>>::element_type;
			
			explicit virtual_reference(const pointer& ptr);

			template <typename OtherElement, process_rights OtherAccess, typename OtherAddressT>
			virtual_reference(const virtual_reference<OtherElement, OtherAccess, OtherAddressT>& other);

			template <typename OtherElement, process_rights OtherAccess, typename OtherAddressT>
			virtual_reference& operator=(const virtual_reference<OtherElement, OtherAccess, OtherAddressT>& other);

			virtual_reference& operator=(const value_type& x);

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
			template <typename OtherElement, process_rights OtherAccess, typename OtherAddressT>
			friend class virtual_reference;

			template <typename OtherElement, process_rights OtherAccess, typename OtherAddressT>
			friend class virtual_ptr;

			const pointer ptr_;

		}; // class virtual_reference

		template <typename Element, process_rights Access, typename AddressT>
		auto make_virtual_reference(const process<Access>& process, const address<AddressT> address = nullptr)
		{ return *make_virtual_ptr<Element, Access, AddressT>(process, address);}

		template <typename Element, process_rights Access>
		auto make_virtual_reference(const process<Access>& process, const address<dword> address = nullptr)
		{ return *make_virtual_ptr<Element, Access>(process, address);}

	} // namespace memory

	/*template <typename Element, typename AddressT = dword>
	using virtual_reference = memory::virtual_reference<Element, vm_rw_op, AddressT>;

	template <typename Element, typename AddressT = dword>
	using const_virtual_reference = memory::virtual_reference<Element, vm_read, AddressT>;*/

	using memory::virtual_reference;
	using memory::make_virtual_reference;

} // namespace distant


#include "impl/virtual_reference.hxx"
