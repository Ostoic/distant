// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once
#include "../virtual_memory.hpp"

#include <distant/memory/type_traits.hpp>
#include <distant/memory/customize_operations.hpp>

namespace distant::memory
{
	template <typename T, typename AddressT>
	BOOST_FORCEINLINE
	void write(process<vm_w_op>& proc, const address<AddressT> address, T&& x)
	{
		customize::write<T>::template 
			invoke<AddressT>(proc, address, std::forward<T>(x));
	}

	template <typename T>
	BOOST_FORCEINLINE
	void write(process<vm_w_op>& proc, const address<dword> address, T&& x)
	{
		customize::write<T>::template
			invoke<dword>(proc, address, std::forward<T>(x));
	}

	template <typename T, typename AddressT>
	BOOST_FORCEINLINE
	T read(const process<vm_read>& process, const address<AddressT> address, std::size_t size)
	{
		return customize::read<T>::template
			invoke<AddressT>(process, address, size);
	}

	template <typename T>
	BOOST_FORCEINLINE
	T read(const process<vm_read>& process, const address<dword> address, std::size_t size)
	{
		return customize::read<T>::template
			invoke<dword>(process, address, size);
	}

	template <typename AddressT>
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
			throw windows_error("[memory::virtual_protect] VirtualProtectEx failed");

		return static_cast<page_protection>(old);
	}

	template <page_protection Protection>
	BOOST_FORCEINLINE
	page_protection virtual_protect(process<vm_op>& process, const address<dword> address, const std::size_t size)
	{
		return virtual_protect<Protection, dword>(process, address, size);
	}

	template <page_protection Protection, typename AddressT>
	BOOST_FORCEINLINE
	page_protection virtual_protect(process<vm_op>& process, const address<AddressT> address, const std::size_t size)
	{
		static_assert(
			detail::has_virtual_protect_support(Protection),
			"[memory::virtual_protect] Selected page_protection is not supported"
		);

		return virual_protect<AddressT>(process, address, Protection, size);
	}

	inline page_protection virtual_protect(process<vm_op>& process, const address<dword> address, const page_protection protection, const std::size_t size)
	{
		return virtual_protect<dword>(process, address, protection, size);
	}

	template <typename AddressT>
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
	//PROCESS_VM_OPERATION
	template <typename T, page_protection Protection, typename AddressT, process_rights AccessRights>
	virtual_ptr<T, AddressT, AccessRights> virtual_malloc(process<AccessRights>& process, const std::size_t n)
	{
		static_assert(
			detail::has_virtual_malloc_support(Protection),
			"[memory::virtual_malloc] Selected page_protection is not supported"
		);

		void* allocated_address = ::VirtualAllocEx(process.handle().native_handle(), nullptr, n, MEM_RESERVE | MEM_COMMIT, static_cast<boost::winapi::DWORD_>(Protection));
		if (allocated_address == nullptr)
			throw windows_error("[memory::virtual_malloc] VirtualAllocEx failed");

		return virtual_ptr<T, AddressT, AccessRights>{process, allocated_address};
	}

	template <typename T, page_protection Protection, process_rights AccessRights>
	virtual_ptr<T, dword, AccessRights> virtual_malloc(process<AccessRights>& process, const std::size_t n)
	{
		return virtual_malloc<T, Protection, dword, AccessRights>(process, n);
	}

	template <typename T, typename AddressT, process_rights AccessRights>
	bool virtual_free(process<AccessRights>& process, const virtual_ptr<T, AddressT, AccessRights> pointer) noexcept
	{
		return ::VirtualFreeEx(
			process.handle().native_handle(), 
			reinterpret_cast<void*>(static_cast<AddressT>(pointer.get())),
			0, MEM_RELEASE
		);
	}
}
