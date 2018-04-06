// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

namespace distant::system
{
	enum class processor_architecture
	{
		amd64 = 9,
		ia64 = 6,
		arm = 5,
		intel = 0,
		unknown = 0xffff,
	};
}
