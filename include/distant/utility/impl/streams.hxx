#pragma once
#include <distant\utility\streams.hpp>

namespace distant::utility
{
	template <typename Stream>
	bool is_hex(const Stream& stream) noexcept
	{
		return (stream.flags() & stream.basefield) == stream.hex;
	}

	template <typename Stream>
	bool is_dec(const Stream& stream) noexcept
	{
		return (stream.flags() & stream.basefield) == stream.dec;
	}

} // namespace distant::utility
