#pragma once
#include <distant/memory/virtual_memory.hpp>

namespace distant::memory
{
	inline virtual_memory::virtual_memory(const process<required_access>& process) noexcept
		: process_(&process) {}

}
