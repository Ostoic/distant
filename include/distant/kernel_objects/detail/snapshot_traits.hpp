// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/support/winapi/toolhelp32.hpp>

#include <distant/detail/tags.hpp>
#include "distant/kernel_objects/unsafe_process.hpp"

namespace distant::kernel_objects::detail {
	
	template <class T>
	struct snapshot_dispatcher {};

	template <access_rights::process access>
	struct snapshot_dispatcher<kernel_objects::process<access>>
	{
		using dispatch = distant::detail::process_tag;
		using entry_type = boost::winapi::PROCESSENTRY32_;
	};

	template <>
	struct snapshot_dispatcher<kernel_objects::unsafe_process>
	{
		using dispatch = distant::detail::process_tag;
		using entry_type = boost::winapi::PROCESSENTRY32_;
	};

} // end namespace distant::system::detail
