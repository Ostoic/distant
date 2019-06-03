// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

namespace distant::system
{
	enum class processor_architecture
	{
		amd64 = 9,
		ia64 = 6,
		arm = 5,
		intel = 0,
		unknown = 0xffff,
	};

	template <typename CharT, typename TraitsT>
	std::basic_ostream<CharT, TraitsT>& 
		operator<<(std::basic_ostream<CharT, TraitsT>& stream, processor_architecture architecture);
} // namespace distant::system

#include "impl/architectures.hxx"