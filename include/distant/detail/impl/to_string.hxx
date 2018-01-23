#pragma once
#include <distant\detail\to_string.hpp>

inline std::ostream& operator<<(std::ostream& stream, distant::access_rights::process access)
{
	using ar = distant::access_rights::process;
	constexpr auto map = meta::make_map(
		std::make_pair(ar::all_access, "all_access"),
		std::make_pair(ar::set_information, "set_information"),
		std::make_pair(ar::set_quota, "set_quota"),
		std::make_pair(ar::vm_operation, "vm_operation"),
		std::make_pair(ar::vm_read, "vm_read"),
		std::make_pair(ar::vm_write, "vm_write"),
		std::make_pair(ar::create_process, "create_process"),
		std::make_pair(ar::create_thread, "create_thread"),
		std::make_pair(ar::dup_handle, "dup_handle"),
		std::make_pair(ar::suspend_resume, "suspend_resume"),
		std::make_pair(ar::terminate, "terminate"),
		std::make_pair(ar::query_limited_information, "query_limited_information"),
		std::make_pair(ar::query_information, "query_information"),
		std::make_pair(ar::synchronize, "synchronize")
	);

	if (map.count(access))
		stream << map[access];

	else
	{
		for (std::size_t i = 0; i < map.size(); ++i)
		{
			const auto pair = *(map.begin() + i);

			if (distant::check_permission(access, pair.first))
			{
				stream << pair.second;
				if (i != map.size() - 1)
					stream << " | ";
			}
		}
	}

	return stream;
}

inline std::wostream& operator<<(std::wostream& stream, distant::access_rights::process access)
{
	using ar = distant::access_rights::process;
	constexpr auto map = meta::make_map(
		std::make_pair(ar::all_access, L"all_access"),
		std::make_pair(ar::set_information, L"set_information"),
		std::make_pair(ar::set_quota, L"set_quota"),
		std::make_pair(ar::vm_operation, L"vm_operation"),
		std::make_pair(ar::vm_read, L"vm_read"),
		std::make_pair(ar::vm_write, L"vm_write"),
		std::make_pair(ar::create_process, L"create_process"),
		std::make_pair(ar::create_thread, L"create_thread"),
		std::make_pair(ar::dup_handle, L"dup_handle"),
		std::make_pair(ar::suspend_resume, L"suspend_resume"),
		std::make_pair(ar::terminate, L"terminate"),
		std::make_pair(ar::query_limited_information, L"query_limited_information"),
		std::make_pair(ar::query_information, L"query_information"),
		std::make_pair(ar::synchronize, L"synchronize")
	);

	if (map.count(access))
		stream << map[access];

	else
	{
		for (std::size_t i = 0; i < map.size(); ++i)
		{
			const auto pair = *(map.begin() + i);

			if (distant::check_permission(access, pair.first))
			{
				stream << pair.second;
				if (i != map.size() - 1)
					stream << L" | ";
			}
		}
	}

	return stream;
}

