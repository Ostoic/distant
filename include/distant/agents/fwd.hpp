// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/access_rights.hpp>

namespace distant::agents
{
	class kernel_object;

	class basic_process;

	template <process_rights R>
	class process;

	template <token_rights R, class Agent>
	class access_token;

	template <typename>
	class snapshot;

	class thread;
}
