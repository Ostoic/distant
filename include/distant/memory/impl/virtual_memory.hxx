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
		void write(const process<vm_w_op>& proc, const address<AddressT> address, T x)
	{
		customize::write<T>::template 
			invoke<AddressT>(proc, address, x);
	}

	template <typename T>
	BOOST_FORCEINLINE
		void write(const process<vm_w_op>& proc, const address<dword> address, T x)
	{
		customize::write<T>::template
			invoke<dword>(proc, address, x);
	}

	template <typename T, typename AddressT>
	BOOST_FORCEINLINE
		T read(const process<vm_read>& process, const address<AddressT> address)
	{
		return customize::read<T>::template
			invoke<AddressT>(process, address);
	}

	template <typename T>
	BOOST_FORCEINLINE
		T read(const process<vm_read>& process, const address<dword> address)
	{
		return customize::read<T>::template
			invoke<dword>(process, address);
	}

	template <typename AddressT>
	page_protection virtual_protect(const process<vm_op>& process, const address<AddressT> address, page_protection protection, const std::size_t size)
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
	page_protection virtual_protect(const process<vm_op>& process, const address<dword> address, const std::size_t size)
	{
		return virtual_protect<Protection, dword>(process, address, size);
	}

	template <page_protection Protection, typename AddressT>
	page_protection virtual_protect(const process<vm_op>& process, const address<AddressT> address, const std::size_t size)
	{
		static_assert(
			detail::has_virtual_protect_support(Protection),
			"[memory::virtual_protect] Selected page_protection is not supported"
		);

		return virual_protect<AddressT>(process, address, Protection, size);
	}

	inline page_protection virtual_protect(const process<vm_op>& process, const address<dword> address, const page_protection protection, const std::size_t size)
	{
		return virtual_protect<dword>(process, address, protection, size);
	}

	template <typename AddressT>
	bool virtual_protect_noexcept(const process<vm_op>& process, const address<AddressT> address, const page_protection protection, const std::size_t size) noexcept
	{
		using boost::winapi::DWORD_;

		DWORD_ old;
		return ::VirtualProtectEx(
			process.handle().native_handle(),
			reinterpret_cast<void*>(static_cast<AddressT>(address)),
			size, static_cast<DWORD_>(protection), &old
		);
	}

	template <typename T, page_protection Protection, process_rights Access, typename AddressT, typename>
	virtual_ptr<T, AddressT> virtual_malloc(const process<Access>& process, const std::size_t n)
	{
		static_assert(
			detail::has_virtual_malloc_support(Protection),
			"[memory::virtual_malloc] Selected page_protection is not supported"
		);

		void* allocated_address = ::VirtualAllocEx(process.handle().native_handle(), nullptr, n, MEM_RESERVE | MEM_COMMIT, static_cast<boost::winapi::DWORD_>(Protection));
		if (allocated_address == nullptr)
			throw windows_error("[memory::virtual_malloc] VirtualAllocEx failed");

		return virtual_ptr<T, AddressT>{process, allocated_address};
	}

	template <typename T, page_protection Protection, process_rights Access, typename>
	virtual_ptr<T, dword> virtual_malloc(const process<Access>& process, const std::size_t n)
	{
		return virtual_malloc<T, Protection, Access, dword, void>(process, n);
	}

	template <typename T>
	bool virtual_free(const process<vm_op>& process, const virtual_ptr<T, dword> pointer) noexcept
	{
		return virtual_free<T, dword>(process, pointer);
	}

	template <typename T, typename AddressT>
	bool virtual_free(const process<vm_op>& process, const virtual_ptr<T, AddressT> pointer) noexcept
	{
		return ::VirtualFreeEx(
			process.handle().native_handle(), 
			reinterpret_cast<void*>(static_cast<AddressT>(pointer.get())),
			0, MEM_RELEASE
		);
	}
}
