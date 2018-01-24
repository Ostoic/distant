#pragma once

#include <distant\access_rights.hpp>
#include <distant\system\architectures.hpp>
#include <distant\memory\address.hpp>

#include <distant\utility\map.hpp>

#include <iostream>
#include <tuple>

std::ostream& operator<<(std::ostream& stream, distant::access_rights::process access);
std::wostream& operator<<(std::wostream& stream, distant::access_rights::process access);

std::ostream& operator<<(std::ostream& stream, distant::system::processor_architecture arch);
std::wostream& operator<<(std::wostream& stream, distant::system::processor_architecture arch);

std::ostream& operator<<(std::ostream& stream, distant::memory::address);
std::wostream& operator<<(std::wostream& stream, distant::memory::address);

#include <distant\detail\impl\to_string.hxx>
