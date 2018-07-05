// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

// Forward declarations of some public (non-implemention) classes.

#include <distant/kernel_objects/fwd.hpp>
#include <distant/error/fwd.hpp>
#include <distant/memory/fwd.hpp>
#include <distant/system/fwd.hpp>
#include <distant/sync/fwd.hpp>

namespace distant 
{
	template <typename HandleTraits>
	class unsafe_handle;

	struct kernel_handle_traits;

	using kernel_handle = unsafe_handle<kernel_handle_traits>;

} // namespace distant