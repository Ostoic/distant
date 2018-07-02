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
		template <typename AddressT = dword, typename T>
		void write(process<vm_w_op>& proc, address<AddressT> address, T&& x);

		template <typename T, typename AddressT = dword>
		T read(const process<vm_read>& process, address<AddressT> address, std::size_t size = sizeof(T));

		template <typename AddressT>
		bool virtual_protect_noexcept(process<vm_op>& process, address<AddressT> address, page_protection protection, std::size_t size) noexcept;

		template <page_protection Protection, typename AddressT = dword>
		page_protection virtual_protect(process<vm_op>& process, address<AddressT> address, std::size_t size);

		template <typename AddressT>
		page_protection virtual_protect(process<vm_op>& process, address<AddressT> address, page_protection protection, std::size_t size);

		template <
			typename T, 
			page_protection Protection = page_protection::execute_readwrite, 
			typename AddressT = dword,
			process_rights AccessRights = process_rights::all_access
		>
		virtual_ptr<T, AddressT, AccessRights> virtual_malloc(process<AccessRights>& process, std::size_t n = sizeof(T));

		template <typename T, typename AddressT, process_rights AccessRights>
		bool virtual_free(process<AccessRights>& process, virtual_ptr<T, AddressT, AccessRights> pointer) noexcept;

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
#include "impl/virtual_memory.hxx"
