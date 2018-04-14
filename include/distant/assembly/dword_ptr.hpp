// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/memory/address.hpp>
#include <distant/assembly/x86_register.hpp>

namespace distant::assembly
{
	template <typename T, typename =
		std::enable_if_t<
			std::is_same_v<x86_register, T> || std::is_same_v<distant::address, T>,
			T
		>
	>
	struct dword_ptr_t
	{
		explicit constexpr dword_ptr_t(T reg_address) noexcept;

		T data;
	};
}

#include <distant/assembly/impl/dword_ptr.hxx>
