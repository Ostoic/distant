#pragma once
#include <distant\detail\to_string.hpp>

namespace distant::detail
{
	using arp = distant::access_rights::process;
	constexpr auto access_rights_names = meta::make_map(
		std::make_pair(arp::all_access, "all_access"),
		std::make_pair(arp::set_information, "set_information"),
		std::make_pair(arp::set_quota, "set_quota"),
		std::make_pair(arp::vm_operation, "vm_operation"),
		std::make_pair(arp::vm_read, "vm_read"),
		std::make_pair(arp::vm_write, "vm_write"),
		std::make_pair(arp::create_process, "create_process"),
		std::make_pair(arp::create_thread, "create_thread"),
		std::make_pair(arp::dup_handle, "dup_handle"),
		std::make_pair(arp::suspend_resume, "suspend_resume"),
		std::make_pair(arp::terminate, "terminate"),
		std::make_pair(arp::query_limited_information, "query_limited_information"),
		std::make_pair(arp::query_information, "query_information"),
		std::make_pair(arp::synchronize, "synchronize")
	);

	using archs = distant::system::processor_architecture;
	constexpr auto arch_names = meta::make_map(
		std::make_pair(archs::amd64, "amd64"),
		std::make_pair(archs::arm, "arm"),
		std::make_pair(archs::ia64, "ia64"),
		std::make_pair(archs::intel, "intel"),
		std::make_pair(archs::unknown, "unknown")
	);
}

inline std::ostream& operator<<(std::ostream& stream, distant::access_rights::process access)
{
	if (distant::detail::access_rights_names.count(access))
		stream << distant::detail::access_rights_names[access];

	else
	{
		for (std::size_t i = 0; i < distant::detail::access_rights_names.size(); ++i)
		{
			const auto pair = *(distant::detail::access_rights_names.begin() + i);

			if (distant::check_permission(access, pair.first))
			{
				if (i != distant::detail::access_rights_names.size() - 1)
					stream << " | ";
			}
		}
	}

	return stream;
}

inline std::wostream& operator<<(std::wostream& stream, distant::access_rights::process access)
{
	using ar = distant::access_rights::process;

	if (distant::detail::access_rights_names.count(access))
		stream << distant::detail::access_rights_names[access];

	else
	{
		for (std::size_t i = 0; i < distant::detail::access_rights_names.size(); ++i)
		{
			const auto pair = *(distant::detail::access_rights_names.begin() + i);

			if (distant::check_permission(access, pair.first))
			{
				stream << pair.second;
				if (i != distant::detail::access_rights_names.size() - 1)
					stream << L" | ";
			}
		}
	}

	return stream;
}

inline std::ostream& operator<<(std::ostream& stream, distant::system::processor_architecture arch)
{
	stream << distant::detail::arch_names[arch];
	return stream;
}

inline std::wostream& operator<<(std::wostream& stream, distant::system::processor_architecture arch)
{
	stream << distant::detail::arch_names[arch];
	return stream;
}

inline std::ostream& operator<<(std::ostream& stream, distant::memory::address address)
{
	std::ios::fmtflags oldFlags{stream.flags()};
	stream << std::hex << std::uppercase;
	stream << "0x" << static_cast<distant::dword>(address);
	stream << std::dec;
	stream.flags(oldFlags);
	return stream;
}

inline std::wostream& operator<<(std::wostream& stream, distant::memory::address address)
{
	std::ios::fmtflags oldFlags{stream.flags()};
	stream << std::hex << std::uppercase;
	stream << "0x" << static_cast<distant::dword>(address);
	stream << std::dec;
	stream.flags(oldFlags);
	return stream;
}