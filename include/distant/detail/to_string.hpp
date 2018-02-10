#pragma once

#include <distant\access_rights.hpp>
#include <distant\system\architectures.hpp>
#include <distant\memory\address.hpp>
#include <distant\memory\opcode.hpp>

#include <distant\utility\meta\map.hpp>

#include <iostream>
#include <tuple>

// forward delcarations
namespace distant::memory
{
	template <std::size_t S, std::size_t C>
	class instruction;
}

template <typename Stream = std::enable_if_t<utility::is_output_stream<Stream>, Stream>>
Stream& operator<<(Stream& stream, distant::access_rights::process access);

template <typename Stream = std::enable_if_t<utility::is_output_stream<Stream>, Stream>>
Stream& operator<<(std::ostream& stream, distant::system::processor_architecture arch);

template <typename Stream = std::enable_if_t<utility::is_output_stream<Stream>, Stream>>
Stream& operator<<(Stream& stream, distant::address);

/******** Assembler instruction section ********/
template <typename Stream = std::enable_if_t<utility::is_output_stream<Stream>, Stream>,
	std::size_t S, std::size_t C
>
Stream& operator<<(Stream& stream, const distant::memory::instruction<S, C>& instr);

// TODO: opcode stream output 
// TODO: Perhaps input as well for all of the above

#include <distant\detail\impl\to_string.hxx>
