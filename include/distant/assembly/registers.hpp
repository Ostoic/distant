// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/types.hpp>

namespace distant::assembly
{
	enum class x86_register : byte
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

	enum class x64_register : byte
	{
		// Data segment
		rax,
		rbx,
		rcx,
		rdx,
		rsi,
		rdi,
		rip,

		// Stack segment
		rbp,
		rsp,
	};

} // namespace distant::assembly
