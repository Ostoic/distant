#pragma once
#include <distant\utility\streams.hpp>

namespace distant::utility
{
	bool is_hex(const std::ostream& stream) noexcept
	{
		return (stream.flags() & stream.basefield) == stream.hex;
	}

	bool is_dec(const std::ostream& stream) noexcept
	{
		return (stream.flags() & stream.basefield) == stream.dec;
	}

	bool is_hex(const std::istream& stream) noexcept
	{
		return (stream.flags() & stream.basefield) == stream.hex;
	}
} // namespace distant::utility
