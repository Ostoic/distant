// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once
#include "../virtual_memory.hpp"

#include <distant/memory/type_traits.hpp>
#include <distant/memory/operations_traits.hpp>
#include <distant/utility/meta/transformations.hpp>

namespace distant::memory
{
	// Note: AddressT is specified first to allow for universal reference type deduction
	template <class AddressT, class T>
	BOOST_FORCEINLINE
	void write(process<vm_w_op>& proc, const address<AddressT> address, T&& x)
	{
		// rv&& -> binds to const&
		// lv& -> binds to const&
		// const& -> binds to const&
		using utility::meta::remove_cvref;
		operations_traits<remove_cvref<T>>::write(proc, address, std::forward<T>(x));
	}

	template <class AddressT, class... Ts>
	std::array<address<AddressT>, sizeof...(Ts)> write_aligned(process<vm_w_op>& proc, address<AddressT> address, Ts&&... ts)
	{
		using utility::meta::remove_cvref;
		return operations_traits<std::pair<std::tuple<remove_cvref<Ts>...>, detail::aligned_tag_t>>
			::write(proc, address, std::forward_as_tuple(std::forward<Ts>(ts)...));
	}

	template <class AddressT, class... Ts>
	std::array<address<AddressT>, sizeof...(Ts)> write_contiguous(process<vm_w_op>& proc, address<AddressT> address, Ts&&... ts)
	{
		using utility::meta::remove_cvref;
		return operations_traits<std::pair<std::tuple<remove_cvref<Ts>...>, detail::contiguous_tag_t>>
			::write(proc, address, std::forward_as_tuple(std::forward<Ts>(ts)...));
	}

	template <class T, class AddressT>
	BOOST_FORCEINLINE
	T read(const process<vm_read>& process, const address<AddressT> address, std::size_t size)
	{
		using utility::meta::remove_cvref;
		return operations_traits<remove_cvref<T>>::read(process, address, size);
	}

	template <class Tuple, class AddressT>
	BOOST_FORCEINLINE
	Tuple read_aligned(const process<vm_read>& process, const address<AddressT> address, std::size_t size)
	{
		using utility::meta::remove_cvref;
		return operations_traits<remove_cvref<Tuple>>::read(process, address, size);
	}

	template <page_protection Protection, typename AddressT>
	BOOST_FORCEINLINE
	page_protection virtual_protect(process<vm_op>& process, const address<AddressT> address, const std::size_t size)
	{
		static_assert(
			detail::has_virtual_protect_support(Protection),
			"[memory::virtual_protect] Selected page_protection is not supported"
		);

		return virtual_protect(process, address, Protection, size);
	}

	template <class AddressT>
	BOOST_FORCEINLINE
	page_protection virtual_protect(process<vm_op>& process, const address<AddressT> address, page_protection protection, const std::size_t size)
	{
		using boost::winapi::DWORD_;

		DWORD_ old;
		if (!::VirtualProtectEx(
			process.handle().native_handle(),
			reinterpret_cast<void*>(static_cast<AddressT>(address)),
			size, static_cast<DWORD_>(protection), &old
		))
			throw winapi_error("[memory::virtual_protect] VirtualProtectEx failed");

		return static_cast<page_protection>(old);
	}

	template <class AddressT>
	bool virtual_protect_noexcept(process<vm_op>& process, const address<AddressT> address, const page_protection protection, const std::size_t size) noexcept
	{
		using boost::winapi::DWORD_;

		DWORD_ old;
		return ::VirtualProtectEx(
			process.handle().native_handle(),
			reinterpret_cast<void*>(static_cast<AddressT>(address)),
			size, static_cast<DWORD_>(protection), &old
		);
	}

	template <page_protection Protection, class AddressT, process_rights AccessRights>
	address<AddressT> virtual_malloc(process<AccessRights>& process, const std::size_t n)
	{
		static_assert(
			detail::has_virtual_malloc_support(Protection),
			"[memory::virtual_malloc] Selected page_protection is not supported"
		);

		void* allocated_address = ::VirtualAllocEx(
			process.handle().native_handle(), 
			nullptr, 
			n, 
			MEM_RESERVE | MEM_COMMIT, 
			static_cast<boost::winapi::DWORD_>(Protection)
		);

		if (allocated_address == nullptr)
			throw winapi_error("[memory::virtual_malloc] VirtualAllocEx failed");

		return address<AddressT>{allocated_address};
	}

	//PROCESS_VM_OPERATION
	template <class T, page_protection Protection, class AddressT, process_rights AccessRights>
	virtual_ptr<T, AddressT, AccessRights> virtual_malloc(process<AccessRights>& process, const std::size_t n)
	{
		return virtual_ptr<T, AddressT, AccessRights>{process, virtual_malloc(process, n)};
	}

	template <class T, page_protection Protection, process_rights AccessRights>
	virtual_ptr<T, dword, AccessRights> virtual_malloc(process<AccessRights>& process, const std::size_t n)
	{
		return virtual_malloc<T, Protection, dword, AccessRights>(process, n);
	}

	template <class AddressT, process_rights AccessRights>
	bool virtual_free(process<AccessRights>& process, address<AddressT> address) noexcept
	{
		return ::VirtualFreeEx(
			process.handle().native_handle(),
			reinterpret_cast<void*>(static_cast<AddressT>(address)),
			0,
			MEM_RELEASE
		);
	}

	template <class T, class AddressT, process_rights AccessRights>
	bool virtual_free(const virtual_ptr<T, AddressT, AccessRights> pointer) noexcept
	{
		return virtual_free(pointer.process(), pointer.get());
	}
}
