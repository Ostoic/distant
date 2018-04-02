#pragma once

#include <distant/kernel/fwd.hpp>

#include <distant/process.hpp>

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
}

// Implementation:
#include <distant/memory/impl/virtual_memory.hxx>
