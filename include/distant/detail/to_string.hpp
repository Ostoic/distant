// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/access_rights.hpp>
#include <distant/system/architectures.hpp>
#include <distant/memory/address.hpp>
#include <distant/assembly/opcode.hpp>

#include <distant/utility/meta/map.hpp>

// forward delcarations
namespace distant::memory
{
	template <std::size_t S, std::size_t C>
	class static_instruction;
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
Stream& operator<<(Stream& stream, const distant::memory::static_instruction<S, C>& instr);

// Implementation:
#include <distant/detail/impl/to_string.hxx>
