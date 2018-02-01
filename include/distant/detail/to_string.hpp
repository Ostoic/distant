#pragma once

#include <distant\access_rights.hpp>
#include <distant\system\architectures.hpp>
#include <distant\memory\address.hpp>
#include <distant\memory\opcode.hpp>

#include <distant\utility\map.hpp>

#include <iostream>
#include <tuple>

// forward delcarations
namespace distant::memory
{
	template <std::size_t S, std::size_t C>
	class instruction;
}

std::ostream& operator<<(std::ostream& stream, distant::access_rights::process access);
std::wostream& operator<<(std::wostream& stream, distant::access_rights::process access);

std::ostream& operator<<(std::ostream& stream, distant::system::processor_architecture arch);
std::wostream& operator<<(std::wostream& stream, distant::system::processor_architecture arch);

std::ostream& operator<<(std::ostream& stream, distant::address);
std::wostream& operator<<(std::wostream& stream, distant::address);


/******** Assembler instruction section ********/
template <std::size_t S, std::size_t C>
std::ostream& operator<<(std::ostream& stream, const distant::memory::instruction<S, C>& instr);

template <std::size_t S, std::size_t C>
std::wostream& operator<<(std::wostream& stream, const distant::memory::instruction<S, C>& instr);

// TODO: opcode stream output 
// TODO: Perhaps input as well for all of the above

#include <distant\detail\impl\to_string.hxx>
