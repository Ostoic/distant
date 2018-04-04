#pragma once

#include <boost/iterator/iterator_adaptor.hpp>

#include <distant/memory/address.hpp>
#include <distant/memory/fwd.hpp>

#include <distant/memory/type_traits.hpp>

namespace distant 
{ 
	namespace memory 
	{
		// Todo: Consider checking the memory for read/write access and throwing on failure in another class
		// this one should be similar to a raw pointer.
		template <typename Element, typename AddressT = dword>
		class virtual_ptr : public
			boost::iterator_facade<
				virtual_ptr<Element, AddressT>,
				Element,	
				std::random_access_iterator_tag,
				virtual_reference<Element, AddressT>,
				address<AddressT>
			>
		{
		public:
			using element_type	  = typename std::pointer_traits<virtual_ptr>::element_type;
			using difference_type = typename std::pointer_traits<virtual_ptr>::difference_type;
			using reference		  = virtual_reference<Element, AddressT>;
			using const_reference = virtual_reference<const std::remove_cv_t<Element>, AddressT>;

			static constexpr auto vm_access = virtual_traits<virtual_ptr>::vm_access;
			
		public:
			virtual_ptr() noexcept;

			virtual_ptr(nullptr_t) noexcept;

			template <process_rights Access, typename = std::enable_if_t<constrain_permission(Access, vm_access)>>
			explicit virtual_ptr(const process<Access>& process, address<AddressT> address = nullptr) noexcept;

			template <
				typename OtherT = std::enable_if_t<detail::required_vm_access<OtherT> == vm_access, OtherT>,
				typename OtherAddressT>
			virtual_ptr(virtual_ptr<OtherT, OtherAddressT> pointer) noexcept;

			address<AddressT> get() const noexcept;

		private:
			friend class boost::iterator_core_access;

			// interface for boost::iterator_facade
			void increment() noexcept;
			void decrement() noexcept;

			const_reference dereference() const;
			reference		dereference();
			
			template <
				typename OtherT = std::enable_if_t<detail::required_vm_access<OtherT> == vm_access, OtherT>,
				typename OtherAddressT>
			bool equal(virtual_ptr<OtherT, OtherAddressT> other) const noexcept;

			void advance(int n) noexcept;

			template <
				typename OtherT = std::enable_if_t<detail::required_vm_access<OtherT> == vm_access, OtherT>,
				typename OtherAddressT>
			difference_type distance_to(virtual_ptr<OtherT, OtherAddressT> other) const noexcept;

		private:
			template <typename E, typename T>
			friend class virtual_reference;

			template <typename E, typename T>
			friend class virtual_ptr;

			const process<vm_access>* process_;
			address<AddressT> address_;
		};

		template <
			typename Element, 
			typename AddressT, 
			process_rights Access,
			typename = std::enable_if_t<constrain_permission(Access, detail::required_vm_access<Element>)>
		>
		auto make_virtual_ptr(const process<Access>& process, const address<AddressT> address = nullptr)
		{ return virtual_ptr<Element, AddressT>(process, address); }


		template <
			typename Element,
			process_rights Access,
			typename = std::enable_if_t<constrain_permission(Access, detail::required_vm_access<Element>)>
		>
		auto make_virtual_ptr(const process<Access>& process, const address<dword> address = nullptr)
		{ return virtual_ptr<Element, dword>(process, address); }

		template<typename T, typename AddressT, typename CharT, typename Traits>
		std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& stream, virtual_ptr<T, AddressT> p)
		{
			stream << std::hex << reinterpret_cast<void*>(static_cast<AddressT>(p.get()));
			return stream;
		}

		template <typename Element, typename AddressT>
		struct virtual_traits<virtual_ptr<Element, AddressT>>
		{
			static constexpr auto vm_access = detail::required_vm_access<Element>;
		};

	} // namespace memory

	using memory::virtual_ptr;
	using memory::make_virtual_ptr;


} // namespace distant

namespace std
{
	template <typename Element, typename AddressT>
	struct pointer_traits<distant::memory::virtual_ptr<Element, AddressT>>
	{
	private:
		template <typename U>
		using vptr_t = distant::memory::virtual_ptr<U, AddressT>;

	public:
		using element_type	  = std::remove_cv_t<Element>;
		using pointer		  = vptr_t<Element>;
		using difference_type = distant::memory::address<AddressT>;

		template <typename U>
		struct rebind
		{
			using type = vptr_t<U>;
		};
			
		static pointer pointer_to(const distant::memory::virtual_reference<element_type, AddressT>& reference)
		{
			return pointer{reference.ptr_};
		}
	};

	template <typename Element, typename AddressT>
	struct iterator_traits<distant::memory::virtual_ptr<Element, AddressT>>
	{
	private:
		using vptr_t = distant::memory::virtual_ptr<Element, AddressT>;

	public:
		using difference_type	= typename pointer_traits<vptr_t>::difference_type;
		using pointer			= typename pointer_traits<vptr_t>::pointer;
		using value_type		= typename pointer_traits<vptr_t>::element_type;
		using reference			= distant::memory::virtual_reference<Element, AddressT>;
		using iterator_category = std::random_access_iterator_tag;
	};
}

// Implementation:
#include "impl/virtual_ptr.hxx"
