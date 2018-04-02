#pragma once

#include <boost/iterator/iterator_adaptor.hpp>

#include <distant/memory/address.hpp>
#include <distant/memory/fwd.hpp>

namespace distant 
{ 
	namespace memory 
	{
		template <typename Element, process_rights Access, typename AddressT = dword>
		class virtual_ptr : public
			boost::iterator_facade<
				virtual_ptr<Element, Access, AddressT>,
				Element,	
				std::random_access_iterator_tag,
				virtual_reference<Element, Access, AddressT>,
				address<AddressT>
			>
		{
		public:
			using element_type	  = typename std::pointer_traits<virtual_ptr>::element_type;
			using difference_type = typename std::pointer_traits<virtual_ptr>::difference_type;
			using reference		   = virtual_reference<Element, Access, AddressT>;

		public:
			virtual_ptr();

			explicit virtual_ptr(const kernel::process<Access>& process, address<AddressT> address = nullptr);

			template <typename OtherT, process_rights OtherAccess, typename OtherAddressT>
			virtual_ptr(const virtual_ptr<OtherT, OtherAccess, OtherAddressT>& pointer);

			address<AddressT> get() const;

		private:
			friend class boost::iterator_core_access;

			// interface for boost::iterator_facade
			void increment();

			reference dereference() const;

			reference dereference();

			template <typename OtherT, process_rights OtherAccess, typename OtherAddressT>
			bool equal(const virtual_ptr<OtherT, OtherAccess, OtherAddressT>& other) const;

			void advance(int n) noexcept;

			template <typename OtherT, process_rights OtherAccess, typename OtherAddressT>
			difference_type distance_to(const virtual_ptr<OtherT, OtherAccess, OtherAddressT>& other) const noexcept;

		private:
			template <typename E, process_rights A, typename T>
			friend class virtual_reference;

			const kernel::process<Access>* process_;
			address<AddressT> address_;
		};

		template <typename Element, process_rights Access>
		auto make_virtual_ptr(const kernel::process<Access>& process, address<dword> address = nullptr)
		{
			return virtual_ptr<Element, Access, dword>{process, address};
		}

	} // namespace memory

	using memory::virtual_ptr;
	using memory::make_virtual_ptr;

} // namespace distant

namespace std
{
	template <typename Element, distant::process_rights Access, typename AddressT>
	struct pointer_traits<distant::memory::virtual_ptr<Element, Access, AddressT>>
	{
	private:
		template <typename U>
		using vptr_t = distant::memory::virtual_ptr<U, Access, AddressT>;

	public:
		using element_type	  = Element;
		using pointer		  = vptr_t<Element>;
		using difference_type = distant::memory::address<AddressT>;

		template <typename U>
		struct rebind
		{
			using type = vptr_t<U>;
		};
			
		static pointer pointer_to(distant::memory::virtual_reference<element_type, Access, AddressT>)
		{
			return ;
		}
	};

	template <typename Element, distant::process_rights Access, typename AddressT>
	struct iterator_traits<distant::memory::virtual_ptr<Element, Access, AddressT>>
	{
	private:
		using vptr_t = distant::memory::virtual_ptr<Element, Access, AddressT>;

	public:
		using difference_type	= typename pointer_traits<vptr_t>::difference_type;
		using pointer			= typename pointer_traits<vptr_t>::pointer;
		using value_type		= typename pointer_traits<vptr_t>::element_type;
		using reference			= distant::memory::virtual_reference<Element, Access, AddressT>;
		using iterator_category = std::random_access_iterator_tag;
	};
}

// Implementation:
#include "impl/virtual_ptr.hxx"
