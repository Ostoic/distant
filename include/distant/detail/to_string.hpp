#pragma once

#include <distant\access_rights.hpp>

#include <distant\utility\map.hpp>

#include <iostream>
#include <tuple>

std::ostream& operator<<(std::ostream& stream, distant::access_rights::process access);
std::wostream& operator<<(std::wostream& stream, distant::access_rights::process access);

#include <distant\detail\impl\to_string.hxx>
