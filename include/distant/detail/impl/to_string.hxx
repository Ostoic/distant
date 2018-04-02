#pragma once
#include <distant/detail/to_string.hpp>

#include <distant/utility/streams.hpp>

#include <iomanip>

#define MAKE_NAME_OPCODE_REG(op, reg) std::make_pair(ops::##op##_##reg##, #op " " #reg)

#define MAKE_NAME_OPCODE_ALL(op) MAKE_NAME_OPCODE_REG(op, eax),\
							   MAKE_NAME_OPCODE_REG(op, ecx),\
							   MAKE_NAME_OPCODE_REG(op, edx),\
							   MAKE_NAME_OPCODE_REG(op, ebx),\
							   MAKE_NAME_OPCODE_REG(op, ebp),\
							   MAKE_NAME_OPCODE_REG(op, esi),\
							   MAKE_NAME_OPCODE_REG(op, edi)


namespace distant::detail::string_maps
{
	using arp = access_rights::process;
	constexpr auto access_rights_names = utility::meta::make_map(
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
	constexpr auto op_names = utility::meta::make_map(
		std::make_pair(ops::call, "call"),
		MAKE_NAME_OPCODE_ALL(call),

		std::make_pair(ops::jmp, "jmp"),
		MAKE_NAME_OPCODE_ALL(jmp),

		// Move opcodes
		MAKE_NAME_OPCODE_ALL(mov),
		std::make_pair(ops::mov_eax_ptr, "mov eax, dword ptr:"),
		std::make_pair(ops::mov_ebp_ptr, "mov ebp, dword ptr:"),
		std::make_pair(ops::mov_ebx_ptr, "mov ebx, dword ptr:"),
		std::make_pair(ops::mov_ecx_ptr, "mov ecx, dword ptr:"),
		std::make_pair(ops::mov_edi_ptr, "mov edi, dword ptr:"),
		std::make_pair(ops::mov_edx_ptr, "mov edx, dword ptr:"),
		std::make_pair(ops::mov_esi_ptr, "mov esi, dword ptr:"),
		std::make_pair(ops::mov_esp_ptr, "mov esp, dword ptr:"),

		// No operation
		std::make_pair(ops::nop, "nop"),

		// Push opcodes
		std::make_pair(ops::push, "push"),
		MAKE_NAME_OPCODE_ALL(push),
		std::make_pair(ops::pushad, "pushad"),
		std::make_pair(ops::pushfd, "pushfd"),

		// Pop opcodes
		MAKE_NAME_OPCODE_ALL(pop),
		std::make_pair(ops::popad, "popad"),
		std::make_pair(ops::popfd, "popfd")
		// TODO: Do the rest
	);

	using archs = system::processor_architecture;
	constexpr auto arch_names = utility::meta::make_map(
		std::make_pair(archs::amd64, "amd64"),
		std::make_pair(archs::arm, "arm"),
		std::make_pair(archs::ia64, "ia64"),
		std::make_pair(archs::intel, "intel"),
		std::make_pair(archs::unknown, "unknown")
	);
}

template <typename Stream>
Stream& operator<<(Stream& stream, const distant::process_rights access)
{
	using distant::detail::string_maps::access_rights_names;
	using ar = distant::access_rights::process;

	if (access_rights_names.count(access))
		stream << access_rights_names[access];

	else
	{
		// Loop through the map and check if each access right is set in the given access.
		for (std::size_t i = 0; i < access_rights_names.size(); ++i)
		{
			const auto pair = *(access_rights_names.begin() + i);

			if (check_permission(access, pair.first))
			{
				stream << pair.second;
				if (i != access_rights_names.size() - 1)
					stream << L" | ";
			}
		}
	}

	return stream;
}

template <typename Stream>
Stream& operator<<(Stream& stream, distant::system::processor_architecture arch)
{
	using distant::detail::string_maps::arch_names;
	stream << arch_names[arch];
	return stream;
}

template <typename Stream>
Stream& operator<<(Stream& stream, const distant::address address)
{
	using char_t = typename Stream::char_type;
	const auto old_flags = stream.flags();

	stream
		<< std::hex << "0x"
		<< std::setfill(char_t{'0'}) << std::setw(2 * sizeof(distant::address))
		<< static_cast<distant::address::address_type>(address);

	stream.flags(old_flags);
	return stream;
}

template <typename Stream, std::size_t S, std::size_t C>
Stream& operator<<(Stream& stream, const distant::memory::static_instruction<S, C>& instr)
{
	using char_t = typename Stream::char_type;
	static_assert(S >= 2, "[operator<<(instruction)] Invalid instruction length");

	using opcode = distant::memory::opcode;
	using distant::detail::string_maps::op_names;

	const auto write_byte = [&stream](const auto&& byte)
	{
		stream << "0x" << std::setw(2) << std::setfill(char_t{'0'})
			<< static_cast<unsigned int>(byte) << ' ';
	};

	const auto old_flags = stream.flags();

	std::size_t i = 0;

	// Combine the first two bytes of the instruction to get the opcode.
	const auto op = static_cast<opcode>(
		distant::make_word(instr[0], instr[1])
	);

	// Test if the opcode is a valid entry among the opcode->name map
	if (op_names.count(op) != 0)
	{
		// Write the name of the opcode.
		if (!distant::utility::is_hex(stream))
		{
			stream << op_names[op] << ' ';
			i = 2;
		}
		else if (instr[1] == 0)
		{
			write_byte(instr[0]);
			i = 2;
		}

		// Write the address as a distant::address to the stream. 
		const auto write_dword = [&i, &op, &instr](Stream& stream)
		{
			stream << distant::address(distant::make_dword(instr[i], instr[i + 1], instr[i + 2], instr[i + 3]));
			i += 4;
		};

		// If the opcode accepts a dword, then the next sizeof(distant::dword) bytes constitute a dword.
		if (distant::memory::ops::parameter_is_dword(op) && i + 3 < instr.size())
		{
			if (distant::memory::ops::is_deref_instruction(op))
			{
				stream << "[";
				write_dword(stream);
				stream << "]";
			}
			else
			{
				write_dword(stream);
				stream << ' ';
			}
		}
	}

	// Numbers will be written in hexadecimal.
	stream << std::hex;

	// Write the rest of the bytes to the stream.
	for (; i < instr.size(); ++i)
		write_byte(instr[i]);

	stream.flags(old_flags);
	return stream;
}
