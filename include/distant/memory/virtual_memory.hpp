// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/process.hpp>
#include <distant/memory/address.hpp>
#include <distant/memory/virtual_ptr.hpp>

namespace distant
{
	namespace memory
	{
		template <typename T, typename AddressT>
		void write(const process<vm_w_op>& proc, address<AddressT> address, T x);

		template <typename T>
		void write(const process<vm_w_op>& proc, address<dword> address, T x);

		template <typename T, typename AddressT>
		T read(const process<vm_read>& proc, address<AddressT> address);

		template <typename T>
		T read(const process<vm_read>& proc, address<dword> address);

		template <page_protection Protection, typename AddressT>
		page_protection virtual_protect(const process<vm_op>& process, address<AddressT> address, std::size_t size);

		template <page_protection Protection>
		page_protection virtual_protect(const process<vm_op>& process, address<dword> address, std::size_t size);

		template <typename T, page_protection Protection = page_protection::execute_readwrite, typename AddressT>
		virtual_ptr<T, AddressT> virtual_malloc(const process<vm_op>& process, std::size_t n = sizeof(T));

		template <typename T, page_protection Protection = page_protection::execute_readwrite>
		virtual_ptr<T, dword> virtual_malloc(const process<vm_op>& process, std::size_t n = sizeof(T));

		template <typename T>
		void virtual_free(const process<vm_op>& process, virtual_ptr<T, dword> pointer) noexcept;

		template <typename T, typename AddressT>
		void virtual_free(const process<vm_op>& process, virtual_ptr<T, AddressT> pointer) noexcept;

		//void 

		/***
	MEMORY_BASIC_INFORMATION info;
	
	VirtualQueryEx(WoW::handle, (LPVOID)address, &info, sizeof(info));
	return info.State == MEM_COMMIT;
		 */

	} // namespace memory

	using memory::virtual_free;
	using memory::virtual_malloc;
	using memory::virtual_protect;

} // namespace memory

// Implementation:
#include <distant/memory/impl/virtual_memory.hxx>
