#pragma once

#include <iostream>

namespace distant::utility
{
	template <typename Stream>
	inline constexpr bool is_output_stream = std::is_base_of<std::ostream, Stream>::value || std::is_base_of_v<std::wostream, Stream>::value;

	template <typename Stream>
	inline constexpr bool is_input_stream = std::is_base_of<std::istream, Stream>::value || std::is_base_of<std::wistream, Stream>::value;

	template <typename Stream>
	inline constexpr bool is_stream = std::is_base_of<std::ios_base, Stream>::value;

	template <typename Stream = std::enable_if_t<is_stream<Stream>, Stream>>
	bool is_hex(const Stream&) noexcept;

	template <typename Stream = std::enable_if_t<is_stream<Stream>, Stream>>
	bool is_dec(const Stream&) noexcept;

} // namespace distant::utility

// Implementation:
#include <distant\utility\impl\streams.hxx>
