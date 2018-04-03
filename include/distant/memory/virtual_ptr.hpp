#pragma once

#include <boost/iterator/iterator_adaptor.hpp>

#include <distant/memory/address.hpp>
#include <distant/memory/fwd.hpp>

namespace distant 
{ 
	namespace memory 
	{

		// Todo: Consider checking the memory for read/write access and throwing on failure.
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
			using reference		  = virtual_reference<Element, Access, AddressT>;
			using const_reference = virtual_reference<const std::remove_cv_t<Element>, Access, AddressT>;

		public:
			virtual_ptr() noexcept;

			explicit virtual_ptr(const process<Access>& process, address<AddressT> address = nullptr) noexcept;

			template <typename OtherT, process_rights OtherAccess, typename OtherAddressT>
			virtual_ptr(const virtual_ptr<OtherT, OtherAccess, OtherAddressT>& pointer) noexcept;

			address<AddressT> get() const noexcept;

		private:
			friend class boost::iterator_core_access;

			// interface for boost::iterator_facade
			void increment() noexcept;
			void decrement() noexcept;

			const_reference dereference() const;
			reference		dereference();

			template <typename OtherT, process_rights OtherAccess, typename OtherAddressT>
			bool equal(const virtual_ptr<OtherT, OtherAccess, OtherAddressT>& other) const noexcept;

			void advance(int n) noexcept;

			template <typename OtherT, process_rights OtherAccess, typename OtherAddressT>
			difference_type distance_to(const virtual_ptr<OtherT, OtherAccess, OtherAddressT>& other) const noexcept;

		private:
			template <typename E, process_rights A, typename T>
			friend class virtual_reference;

			template <typename E, process_rights A, typename T>
			friend class virtual_ptr;

			const process<Access>* process_;
			address<AddressT> address_;
		};

		template <typename Element, process_rights Access, typename AddressT>
		auto make_virtual_ptr(const process<Access>& process, const address<AddressT> address = nullptr)
		{ return virtual_ptr<Element, Access, AddressT>{process, address}; }

		template <typename Element, process_rights Access>
		auto make_virtual_ptr(const process<Access>& process, const address<dword> address = nullptr)
		{ return virtual_ptr<Element, Access, dword>{process, address}; }

		template<typename T, process_rights Access, typename AddressT, typename CharT, typename Traits>
		std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& stream, const virtual_ptr<T, Access, AddressT>& p)
		{
			stream << std::hex << reinterpret_cast<void*>(static_cast<AddressT>(p.get()));
			return stream;
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
		using element_type	  = std::remove_cv_t<Element>;
		using pointer		  = vptr_t<Element>;
		using difference_type = distant::memory::address<AddressT>;

		template <typename U>
		struct rebind
		{
			using type = vptr_t<U>;
		};
			
		static pointer pointer_to(const distant::memory::virtual_reference<element_type, Access, AddressT>& reference)
		{
			return pointer{reference.ptr_};
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
