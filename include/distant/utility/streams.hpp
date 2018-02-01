#pragma once

#include <iostream>

namespace distant::utility
{
	bool is_hex(const std::ostream&) noexcept;
	bool is_hex(const std::istream&) noexcept;

	bool is_dec(const std::ostream&) noexcept;
	bool is_dec(const std::istream&) noexcept; // TODO
} // namespace distant::utility

// Implementation:
#include <distant\utility\impl\streams.hxx>
