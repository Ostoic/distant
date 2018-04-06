// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/access_rights.hpp>

namespace distant::kernel
{
	class kernel_object;
	class process_base;

	template <process_rights R>
	class process;

	template <token_rights R, typename Object>
	class access_token;
}
