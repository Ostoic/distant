// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <boost/iterator/iterator_adaptor.hpp>

#include <distant/memory/address.hpp>
#include <distant/memory/fwd.hpp>

#include <distant/memory/type_traits.hpp>

namespace distant 
{ 
	namespace memory 
	{
		template <typename Element, typename AddressT = dword>
		class virtual_ptr : public
			boost::iterator_facade<
				virtual_ptr<Element, AddressT>,
				typename std::pointer_traits<virtual_ptr<Element, AddressT>>::element_type,
				std::random_access_iterator_tag,
				virtual_reference<Element, AddressT>,
				typename std::pointer_traits<virtual_ptr<Element, AddressT>>::difference_type
			>
		{
		public:
			using element_type	  = typename std::pointer_traits<virtual_ptr>::element_type;
			using difference_type = typename std::pointer_traits<virtual_ptr>::difference_type;
			using reference		  = virtual_reference<Element, AddressT>;
			using const_reference = virtual_reference<const std::remove_cv_t<Element>, AddressT>;

			static constexpr auto vm_access = virtual_traits<virtual_ptr>::vm_access;

			template <typename T>
			using require_vm_access_to = std::enable_if_t<(detail::required_vm_access<T>::value >= vm_access) && std::is_convertible<T, Element>::value, T>;
			
		public:
			virtual_ptr() noexcept = default;

			virtual_ptr(nullptr_t) noexcept;

			explicit virtual_ptr(const process<vm_access>& process, address<AddressT> address = 0) noexcept;

			template <
				typename OtherT,
				typename OtherAddressT,
				typename = require_vm_access_to<OtherT>
			>
			virtual_ptr(virtual_ptr<OtherT, OtherAddressT> pointer) noexcept;

			address<AddressT> get() const noexcept;

			const kernel_objects::process<vm_access>& ambient_process() const noexcept { return *process_; }

		private:
			friend class boost::iterator_core_access;

			struct enabler {};

			// interface for boost::iterator_facade
			void increment() noexcept;
			void decrement() noexcept;

			reference dereference() const noexcept;
			
			template <
				typename OtherT,
				typename OtherAddressT,
				typename = require_vm_access_to<OtherT>
			>
			bool equal(virtual_ptr<OtherT, OtherAddressT> other) const noexcept;

			void advance(difference_type n) noexcept;

			template <
				typename OtherT,
				typename OtherAddressT,
				typename = require_vm_access_to<OtherT>
			>
			difference_type distance_to(virtual_ptr<OtherT, OtherAddressT> other) const noexcept;

		private:
			template <typename E, typename T>
			friend class virtual_reference;

			template <typename E, typename T>
			friend class virtual_ptr;

			const kernel_objects::process<vm_access>* process_;
			address<AddressT> address_;
		};

		template <typename Element, typename AddressT = dword>
		bool operator==(virtual_ptr<Element, AddressT> lhs, nullptr_t)
		{ return lhs.get() == 0; }

		template <typename Element, typename AddressT = dword>
		bool operator==(nullptr_t, virtual_ptr<Element, AddressT> rhs)
		{ return rhs.get() == 0; }

		template <typename Element, typename AddressT = dword>
		bool operator!=(virtual_ptr<Element, AddressT> lhs, nullptr_t)
		{ return !operator==(lhs, nullptr); }

		template <typename Element, typename AddressT = dword>
		bool operator!=(nullptr_t, virtual_ptr<Element, AddressT> rhs)
		{ return !operator==(nullptr, rhs); }

		template <typename Element, typename AddressT>
		virtual_ptr<Element, AddressT> 
		make_virtual_ptr(const process<detail::required_vm_access<Element>::value>& p, const address<AddressT> address = nullptr) noexcept
		{
			virtual_ptr<Element, AddressT> result(p, address);
			return result;
		}

		template <typename Element>
		virtual_ptr<Element, dword> 
		make_virtual_ptr(const process<detail::required_vm_access<Element>::value>& p, const address<dword> address = 0) noexcept
		{
			virtual_ptr<Element, dword> result(p, address);
			return result;
		}

		template<typename T, typename AddressT, typename CharT, typename Traits>
		std::basic_ostream<CharT, Traits>& 
			operator<<(std::basic_ostream<CharT, Traits>& stream, virtual_ptr<T, AddressT> p)
		{
			stream << std::hex << reinterpret_cast<void*>(static_cast<AddressT>(p.get()));
			return stream;
		}

		template <typename Element, typename AddressT>
		struct virtual_traits<virtual_ptr<Element, AddressT>>
		{
			static constexpr auto vm_access = detail::required_vm_access<Element>::value;
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
		using difference_type = std::ptrdiff_t;

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
