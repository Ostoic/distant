#pragma once

#include <distant/kernel/fwd.hpp>

#include <distant/process.hpp>
#include <distant/memory/address.hpp>

namespace distant::memory
{
	class virtual_memory
	{
	public:
		static constexpr auto required_access = process_rights::vm_operation | process_rights::vm_read | process_rights::vm_write;

	public:
		virtual_memory() = default;

		explicit virtual_memory(const process<required_access>& process) noexcept;

	private:
		const process<required_access>* process_;
	};

	template <typename T, typename AddressT = dword>
	void write(const process<process_rights::vm_write | process_rights::vm_operation>& proc, address<AddressT> address, T x);

	template <typename T, typename AddressT = dword>
	T read(const process<process_rights::vm_read>& proc, address<AddressT> address);

} // namespace distant::memory

// Implementation:
#include <distant/memory/impl/virtual_memory.hxx>
