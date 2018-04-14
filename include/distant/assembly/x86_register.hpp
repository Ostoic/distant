// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/types.hpp>

namespace distant::assembly
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
