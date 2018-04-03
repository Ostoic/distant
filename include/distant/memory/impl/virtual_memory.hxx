#pragma once
#include <distant/memory/virtual_memory.hpp>

#include <distant/memory/type_traits.hpp>

namespace distant::memory
{
	template <typename T, typename AddressT, process_rights Access>
	void write(const process<Access>& proc, const address<AddressT> address, T x)
	{
		static_assert(
			check_permission(Access, process_rights::vm_write | process_rights::vm_operation),
			"[memory::write] vm_write and vm_operation process_rights required"
		);

		SIZE_T bytes_read = 0;
		T buffer = std::move(x);
		if (!::WriteProcessMemory(proc.get_handle().native_handle(), reinterpret_cast<LPVOID>(static_cast<AddressT>(address)), &buffer, sizeof(T), &bytes_read))
			throw std::system_error(distant::last_error(), "[memory::write] WriteProcessMemory failed, " + std::to_string(bytes_read) + " bytes written");
	}


	template <typename T, process_rights Access>
	void write(const process<Access>& proc, const address<dword> address, T x)
	{
		static_assert(
			check_permission(Access, process_rights::vm_write | process_rights::vm_operation),
			"[memory::write] vm_write and vm_operation process_rights required"
		);

		SIZE_T bytes_read = 0;
		T buffer = std::move(x);
		if (!::WriteProcessMemory(proc.get_handle().native_handle(), reinterpret_cast<LPVOID>(static_cast<dword>(address)), &buffer, sizeof(T), &bytes_read))
			throw std::system_error(distant::last_error(), "[memory::write] WriteProcessMemory failed, " + std::to_string(bytes_read) + " bytes written");
	}

	template <typename T, typename AddressT, process_rights Access>
	T read(const process<Access>& process, const address<AddressT> address)
	{
		static_assert(
			check_permission(Access, process_rights::vm_read),
			"[memory::read] vm_operation process_rights required"
		);

		T result;
		SIZE_T bytes_read = 0;

		if (!::ReadProcessMemory(process.get_handle().native_handle(), 
								 reinterpret_cast<LPVOID>(static_cast<AddressT>(address)), 
								 &result, sizeof(T), &bytes_read
		))
			throw std::system_error(distant::last_error(), std::string("[memory::read] ReadProcessMemory failed, ") + std::to_string(bytes_read) + " bytes read");

		return result;
	}

	template <typename T, process_rights Access>
	T read(const process<Access>& proc, const address<dword> address)
	{
		return read<T, dword>(proc, address);
	}

	template <page_protection Protection, process_rights Access>
	page_protection virtual_protect(const process<Access>& process, const address<dword> address, const std::size_t size)
	{
		return virtual_protect<Protection, Access, dword>(process, address, size);
	}

	template <page_protection Protection, process_rights Access, typename AddressT>
	page_protection virtual_protect(const process<Access>& process, const address<AddressT> address, const std::size_t size)
	{
		using boost::winapi::DWORD_;
		static_assert(
			check_permission(Access, process_rights::vm_operation),
			"[memory::virtual_protect] vm_operation process_rights required"
		);

		static_assert(
			detail::has_virtual_protect_support(Protection),
			"[memory::virtual_protect] Selected page_protection is not supported"
		);

		DWORD_ old;
		if (!::VirtualProtectEx(
					process.get_handle().native_handle(), 
					reinterpret_cast<void*>(static_cast<AddressT>(address)), 
					size, static_cast<DWORD_>(Protection), &old
		))
			throw std::system_error(distant::last_error(), "[memory::virtual_protect] VirtualProtectEx failed");

		return static_cast<page_protection>(old);
	}


	template <typename T, page_protection Protection, typename AddressT, process_rights Access>
	virtual_ptr<T, Access, AddressT> virtual_malloc(const process<Access>& process, const std::size_t n)
	{
		static_assert(
			check_permission(Access, process_rights::vm_operation),
			"[memory::virtual_malloc] vm_operation process_rights required"
		);

		static_assert(
			detail::has_virtual_malloc_support(Protection),
			"[memory::virtual_malloc] Selected page_protection is not supported"
		);

		void* result = ::VirtualAllocEx(process.get_handle().native_handle(), nullptr, n, MEM_RESERVE | MEM_COMMIT, static_cast<boost::winapi::DWORD_>(Protection));
		if (result == nullptr)
			throw std::system_error(distant::last_error(), "[memory::virtual_malloc] VirtualAllocEx failed");

		return virtual_ptr<T, Access, AddressT>{process, result};
	}

	template <typename T, page_protection Protection, process_rights Access>
	virtual_ptr<T, Access, dword> virtual_malloc(const process<Access>& process, const std::size_t n)
	{
		return virtual_malloc<T, Protection, dword, Access>(process, n);
	}

	template <typename T, process_rights Access>
	void virtual_free(const process<Access>& process, const virtual_ptr<T, Access, dword> pointer) noexcept
	{
		virtual_free<T, dword, Access>(process, pointer);
	}

	template <typename T, typename AddressT, process_rights Access>
	void virtual_free(const process<Access>& process, const virtual_ptr<T, Access, AddressT> pointer) noexcept
	{
		static_assert(
			check_permission(Access, process_rights::vm_operation),
			"[memory::virtual_free] vm_operation process_rights required"
		);

		::VirtualFreeEx(
			process.get_handle().native_handle(), 
			reinterpret_cast<void*>(static_cast<AddressT>(pointer.get())),
			0, MEM_RELEASE
		);
	}
}
