#pragma once

#include <distant\support\winapi\toolhelp32.hpp>

#include <distant\detail\tags.hpp>

namespace distant::system::detail {
	
	template <class T>
	struct snapshot_dispatcher {};

	template <access_rights::process access>
	struct snapshot_dispatcher<kernel::process<access>>
	{
		using dispatch = distant::detail::process_tag;
		using entry_type = boost::winapi::PROCESSENTRY32_;
	};

	template <>
	struct snapshot_dispatcher<kernel::process_base>
	{
		using dispatch = distant::detail::process_tag;
		using entry_type = boost::winapi::PROCESSENTRY32_;
	};

} // end namespace distant::system::detail
