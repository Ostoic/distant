#pragma once
#include "../access_rights.hpp"

#include <distant/utility/meta/map.hpp>

namespace distant
{
	namespace detail
	{
		constexpr auto access_rights_names = utility::meta::make_map(
			std::make_pair(process_rights::all_access, "all_access"),
			std::make_pair(process_rights::set_information, "set_information"),
			std::make_pair(process_rights::set_quota, "set_quota"),
			std::make_pair(process_rights::vm_operation, "vm_operation"),
			std::make_pair(process_rights::vm_read, "vm_read"),
			std::make_pair(process_rights::vm_write, "vm_write"),
			std::make_pair(process_rights::create_process, "create_process"),
			std::make_pair(process_rights::create_thread, "create_thread"),
			std::make_pair(process_rights::dup_handle, "dup_handle"),
			std::make_pair(process_rights::suspend_resume, "suspend_resume"),
			std::make_pair(process_rights::terminate, "terminate"),
			std::make_pair(process_rights::query_limited_information, "query_limited_information"),
			std::make_pair(process_rights::query_information, "query_information"),
			std::make_pair(process_rights::synchronize, "syncronize")
		);
	}

	template <typename CharT, typename TraitsT>
	std::basic_ostream<CharT, TraitsT>& operator<<(std::basic_ostream<CharT, TraitsT>& stream, const process_rights access)
	{
		using distant::detail::access_rights_names;
		
		bool first_printed = false;

		const auto print_pair = [&first_printed, access, &stream](const auto&& pair)
		{
			if (access >= pair.first)
			{
				// If we've already printed a process_right, we need to start chaining pipe characters after.
				if (first_printed)
					stream << L" | ";
				else 
					first_printed = true;

				stream << pair.second;
				return true;
			}

			return false;
		};

		if (access_rights_names.count(access))
			stream << access_rights_names[access];

		else
			// Loop through the first size-1 elements in the map and check if each access right is set in the given access.
			utility::meta::for_each_tuple(access_rights_names, print_pair);

		return stream;
	}
}