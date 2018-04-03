#pragma once

#include <distant/process.hpp>
#include <distant/memory/address.hpp>
#include <distant/memory/virtual_ptr.hpp>

namespace distant
{
	namespace memory
	{
		template <typename T, typename AddressT, process_rights Access>
		void write(const process<Access>& proc, address<AddressT> address, T x);

		template <typename T, process_rights Access>
		void write(const process<Access>& proc, address<dword> address, T x);

		template <typename T, typename AddressT, process_rights Access>
		T read(const process<Access>& proc, address<AddressT> address);

		template <typename T, process_rights Access>
		T read(const process<Access>& proc, address<dword> address);

		template <page_protection Protection, process_rights Access, typename AddressT>
		page_protection virtual_protect(const process<Access>& process, address<AddressT> address, std::size_t size);

		template <page_protection Protection, process_rights Access>
		page_protection virtual_protect(const process<Access>& process, address<dword> address, std::size_t size);

		template <typename T, page_protection Protection = page_protection::execute_readwrite, typename AddressT, process_rights Access>
		virtual_ptr<T, Access, AddressT> virtual_malloc(const process<Access>& process, std::size_t n = sizeof(T));

		template <typename T, page_protection Protection = page_protection::execute_readwrite, process_rights Access>
		virtual_ptr<T, Access, dword> virtual_malloc(const process<Access>& process, std::size_t n = sizeof(T));

		template <typename T, process_rights Access>
		void virtual_free(const process<Access>& process, virtual_ptr<T, Access, dword> pointer) noexcept;

		template <typename T, typename AddressT, process_rights Access>
		void virtual_free(const process<Access>& process, virtual_ptr<T, Access, AddressT> pointer) noexcept;

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
