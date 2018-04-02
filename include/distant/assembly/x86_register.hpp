#pragma once

#include <distant/types.hpp>

namespace distant::memory
{
	enum class x86_register : distant::byte
	{
		// Data segment
		eax,
		ebx,
		ecx,
		edx,
		esi,
		edi,
		eip,

		// Stack segment
		ebp,
		esp,
	};
}
