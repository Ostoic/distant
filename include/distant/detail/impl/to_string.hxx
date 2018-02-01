#pragma once
#include <distant\detail\to_string.hpp>

#include <distant\utility\streams.hpp>

#include <iomanip>

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

	using ops = memory::opcode;
	constexpr auto op_names = meta::make_map(
		std::make_pair(ops::call_32bit, "call"),
		std::make_pair(ops::jmp, "jmp"),
		std::make_pair(ops::nop, "nop"),
		std::make_pair(ops::push, "push"),
		std::make_pair(ops::pushad, "pushad")
		// TODO: Do the rest
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

inline std::ostream& operator<<(std::ostream& stream, distant::process_rights access)
{
	if (distant::detail::access_rights_names.count(access))
		stream << distant::detail::access_rights_names[access];

	else
	{
		// Loop through the map and check if each access right is set in the given access.
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

inline std::wostream& operator<<(std::wostream& stream, distant::process_rights access)
{
	using ar = distant::access_rights::process;

	if (distant::detail::access_rights_names.count(access))
		stream << distant::detail::access_rights_names[access];

	else
	{
		// Loop through the map and check if each access right is set in the given access.
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

inline std::ostream& operator<<(std::ostream& stream, distant::address address)
{
	const auto oldFlags = stream.flags();
	stream 
		<< std::hex << std::uppercase << std::showbase
		<< std::setfill('0') << std::setw(2 * sizeof(distant::address))
		<< static_cast<distant::address::address_type>(address)
		<< std::dec;

	stream.flags(oldFlags);
	return stream;
}

inline std::wostream& operator<<(std::wostream& stream, distant::address address)
{
	const auto oldFlags = stream.flags();
	stream
		<< std::hex << std::uppercase << std::showbase
		<< std::setfill(L'0') << std::setw(2 * sizeof(distant::address))
		<< static_cast<distant::address::address_type>(address)
		<< std::dec;

	stream.flags(oldFlags);
	return stream;
}

template <std::size_t S, std::size_t C>
std::ostream& operator<<(std::ostream& stream, const distant::memory::instruction<S, C>& instr)
{
	using opcode = distant::memory::opcode;
	using distant::detail::op_names;

	const auto oldFlags = stream.flags();
	if (!distant::utility::is_hex(stream))
	{
		const opcode op = static_cast<opcode>(instr[0]);

		std::size_t i = (op_names.count(op) > 0) ? 1 : 0;

		if (i > 0)
			stream << op_names[op];

		for (; i < instr.size(); ++i)
		{
			stream << ' ';
			stream 
				<< std::hex << "0x" << std::setfill('0') << std::setw(2)
				<< static_cast<unsigned int>(instr[i]);
		}
	}
	else
	{
		for (std::size_t i = 0; i < instr.size(); ++i)
			stream
				<< std::showbase << std::setfill('0') << std::setw(2)
				<< static_cast<unsigned int>(instr[i]) << ' ';
	}

	stream.flags(oldFlags);
	return stream;
}

template <std::size_t S, std::size_t C>
std::wostream& operator<<(std::wostream& stream, const distant::memory::instruction<S, C>& instr)
{
	const auto oldFlags = stream.flags();
	
	for (std::size_t i = 0; i < instr.size(); ++i)
		stream 
			<< std::showbase << std::hex << std::setfill('0') << std::setw(2)
			<< static_cast<unsigned int>(instr[i]) << ' ';

	stream.flags(oldFlags);
	return stream;
}