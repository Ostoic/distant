// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <boost/iterator/iterator_adaptor.hpp>

#include <distant/memory/address.hpp>
#include <distant/memory/fwd.hpp>
#include <distant/access_rights.hpp>

#include <distant/memory/type_traits.hpp>

namespace distant::memory
{
	template <class Element, class AddressT, process_rights AccessRights>
	class virtual_ptr : public
		boost::iterator_facade<
			virtual_ptr<Element, AddressT, AccessRights>,
			typename std::pointer_traits<virtual_ptr<Element, AddressT, AccessRights>>::element_type,
			std::random_access_iterator_tag,
			virtual_reference<Element, AddressT, AccessRights>,
			typename std::pointer_traits<virtual_ptr<Element, AddressT, AccessRights>>::difference_type
		>
	{
	public:
		using value_type		= Element;
		using element_type		= typename std::pointer_traits<virtual_ptr>::element_type;
		using difference_type	= typename std::pointer_traits<virtual_ptr>::difference_type;
		using reference			= virtual_reference<Element, AddressT, AccessRights>;
		using iterator_category = std::random_access_iterator_tag;

		using address_t = address<AddressT>;

		static constexpr auto vm_access =		   virtual_traits<virtual_ptr>::vm_access;
		using process_type			    = typename virtual_traits<virtual_ptr>::process_type;

		template <typename T>
		struct require_vm_access_to
		{
			static constexpr bool value = (detail::required_vm_access<T>::value >= vm_access) && std::is_convertible<T, Element>::value;
		};

		// Check if the given process rights are compatible with the required access.
		//static_assert(AccessRights >= vm_access, "[virtual_ptr] Unable to create a virtual_ptr in a process without proper security access");
	public:
		constexpr virtual_ptr() noexcept : process_(nullptr) {}

		constexpr virtual_ptr(nullptr_t) noexcept;

		explicit virtual_ptr(process_type& process, address_t address = 0) noexcept;

		template <
			class OtherT,
			class OtherAddressT,
			process_rights OtherAccess,
			class = std::enable_if_t<std::is_convertible<OtherT, Element>, OtherAddressT>
		>
		virtual_ptr(virtual_ptr<OtherT, OtherAddressT, OtherAccess> pointer) noexcept;

		address_t get() const noexcept;

		process_type& process()		  noexcept { return *process_; }
		process_type& process() const noexcept { return *process_; }

		explicit operator bool () const noexcept
		{
			return address_ != address_t{} && process_ != nullptr;
		}

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
			process_rights OtherAccess
		>
		bool equal(virtual_ptr<OtherT, OtherAddressT, OtherAccess> other) const noexcept;

		void advance(difference_type n) noexcept;

		template <
			typename OtherT,
			typename OtherAddressT,
			process_rights OtherAccess
		>
		difference_type distance_to(virtual_ptr<OtherT, OtherAddressT, OtherAccess> other) const noexcept;

	private:
		template <typename E, typename T, process_rights R>
		friend class virtual_reference;

		template <typename E, typename T, process_rights R>
		friend class virtual_ptr;

		process_type* process_;
		address_t address_;
	};

	template <typename Element, typename AddressT = dword, process_rights AccessRights>
	bool operator==(virtual_ptr<Element, AddressT, AccessRights> lhs, nullptr_t)
	{ return lhs.get() == 0; }

	template <typename Element, typename AddressT = dword, process_rights AccessRights>
	bool operator==(nullptr_t, virtual_ptr<Element, AddressT, AccessRights> rhs)
	{ return rhs.get() == 0; }

	template <typename Element, typename AddressT = dword, process_rights AccessRights>
	bool operator!=(virtual_ptr<Element, AddressT, AccessRights> lhs, nullptr_t)
	{ return !operator==(lhs, nullptr); }

	template <typename Element, typename AddressT = dword, process_rights AccessRights>
	bool operator!=(nullptr_t, virtual_ptr<Element, AddressT, AccessRights> rhs)
	{ return !operator==(nullptr, rhs); }

	template <typename Element, typename AddressT, process_rights AccessRights>
	virtual_ptr<Element, AddressT, AccessRights>
		make_virtual_ptr(process<AccessRights>& p, const address<AddressT> address) noexcept
	{
		virtual_ptr<Element, AddressT, AccessRights> result(p, address);
		return result;
	}

	template <typename Element, typename AddressT, process_rights AccessRights>
	virtual_ptr<Element, AddressT, AccessRights>
		make_virtual_ptr(const process<AccessRights>& p, const address<AddressT> address) noexcept
	{
		virtual_ptr<Element, AddressT, AccessRights> result(p, address);
		return result;
	}

	template <typename Element, process_rights AccessRights>
	virtual_ptr<Element, dword, AccessRights>
		make_virtual_ptr(process<AccessRights>& p, const address<dword> address) noexcept
	{
		virtual_ptr<Element, dword, AccessRights> result(p, address);
		return result;
	}

	template <typename Element, process_rights AccessRights>
	virtual_ptr<Element, dword, AccessRights>
		make_virtual_ptr(const process<AccessRights>& p, const address<dword> address) noexcept
	{
		virtual_ptr<Element, dword, AccessRights> result(p, address);
		return result;
	}

	template<typename T, typename AddressT, typename CharT, typename Traits, process_rights AccessRights>
	std::basic_ostream<CharT, Traits>&
		operator<<(std::basic_ostream<CharT, Traits>& stream, virtual_ptr<T, AddressT, AccessRights> p)
	{
		stream << std::hex << reinterpret_cast<void*>(static_cast<AddressT>(p.get()));
		return stream;
	}

	template <typename Element, process_rights AccessRights, typename AddressT>
	struct virtual_traits<virtual_ptr<Element, AddressT, AccessRights>>
	{
		static constexpr auto vm_access = detail::required_vm_access<Element>::value;

		using process_type = std::conditional_t<std::is_const<Element>::value,
			const process<AccessRights>,
				  process<AccessRights>
		>;
	};

} // namespace distant::memory

namespace distant
{
	using memory::virtual_ptr;
	using memory::make_virtual_ptr;
}

namespace std
{
	template <typename Element, typename AddressT, distant::process_rights AccessRights>
	struct pointer_traits<distant::memory::virtual_ptr<Element, AddressT, AccessRights>>
	{
	private:
		template <typename U>
		using vptr_t = distant::memory::virtual_ptr<U, AddressT, AccessRights>;

	public:
		using element_type	  = std::remove_cv_t<Element>;
		using pointer		  = vptr_t<Element>;
		using difference_type = std::ptrdiff_t;
		//using difference_type = distant::memory::address<AddressT>;

		template <typename U>
		struct rebind
		{
			using type = vptr_t<U>;
		};

		static pointer pointer_to(distant::memory::virtual_reference<element_type, AddressT, AccessRights> reference)
		{
			return pointer{reference.ptr_};
		}
	};
}

// Implementation:
#include "impl/virtual_ptr.hxx"
