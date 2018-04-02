#pragma once
#include <distant/memory/virtual_memory.hpp>

namespace distant::memory
{
	inline virtual_memory::virtual_memory(const process<required_access>& process) noexcept
		: process_(&process) {}

	template <typename T, typename AddressT>
	void write(const process<process_rights::vm_write | process_rights::vm_operation>& proc, address<AddressT> address, T x)
	{
		T buffer = std::move(x);
		if (!::WriteProcessMemory(proc.get_handle().native_handle(), reinterpret_cast<LPVOID>(static_cast<AddressT>(address)), &buffer, sizeof(T), nullptr))
			throw std::system_error(distant::last_error(), "[memory::write] WriteProcessMemory failed");
	}

	template <typename T, typename AddressT>
	T read(const process<process_rights::vm_read>& proc, address<AddressT> address)
	{
		T result;
		if (!::ReadProcessMemory(proc.get_handle().native_handle(), 
								 reinterpret_cast<LPVOID>(static_cast<AddressT>(address)), 
								 &result, sizeof(T), nullptr
		))
			throw std::system_error(distant::last_error(), "[memory::read] ReadProcessMemory failed");

		return result;
	}

}
