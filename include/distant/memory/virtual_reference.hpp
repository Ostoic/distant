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
			using pointer = typename std::pointer_traits<virtual_ptr<Element, Access, AddressT>>::pointer;
			using value_type = typename std::pointer_traits<virtual_ptr<Element, Access, AddressT>>::element_type;

			explicit virtual_reference(const pointer& ptr);

			template <typename OtherElement, process_rights OtherAccess, typename OtherAddressT>
			virtual_reference(const virtual_reference<OtherElement, OtherAccess, OtherAddressT>& other);

			virtual_reference& operator=(const virtual_reference& other);

			template <typename OtherElement, process_rights OtherAccess, typename OtherAddressT>
			virtual_reference& operator=(const virtual_reference<OtherElement, OtherAccess, OtherAddressT>& other);

			virtual_reference& operator=(const value_type& x);

			pointer operator&() const;

			operator value_type() const;

			void swap(virtual_reference& other);

			// These operators can't rely on an implicitly converted implementation since they act as member functions.
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

	} // namespace memory

	using memory::virtual_reference;

} // namespace distant


#include "impl/virtual_reference.hxx"
