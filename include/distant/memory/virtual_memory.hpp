#pragma once

#include <distant\kernel\fwd.hpp>

namespace distant::memory
{
	class virtual_memory
	{
		using process_type = kernel::process<process_rights::vm_operation>;

	public:
		virtual_memory();

		explicit virtual_memory(const process_type& process) noexcept;

	private:
		const kernel::process<process_rights::vm_operation>* process_;
	};
}
