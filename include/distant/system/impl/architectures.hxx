#pragma once
#include "../architectures.hpp"

#include <distant/utility/meta/map.hpp>

namespace distant::system
{
	namespace detail
	{
		constexpr auto arch_names = utility::meta::make_map(
			std::make_pair(processor_architecture::amd64, "amd64"),
			std::make_pair(processor_architecture::arm, "arm"),
			std::make_pair(processor_architecture::ia64, "ia64"),
			std::make_pair(processor_architecture::intel, "intel"),
			std::make_pair(processor_architecture::unknown, "unknown")
		);
	}

	template <typename CharT, typename TraitsT>
	std::basic_ostream<CharT, TraitsT>& 
		operator<<(std::basic_ostream<CharT, TraitsT>& stream, const processor_architecture architecture)
	{
		stream << detail::arch_names[architecture];
		return stream;
	}

}
