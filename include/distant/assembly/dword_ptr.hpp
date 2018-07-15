// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/memory/address.hpp>
#include <distant/assembly/registers.hpp>

namespace distant::assembly
{
	template <class T>
	struct dword_ptr_t;

	template <>
	struct dword_ptr_t<x86_register>
	{
		explicit constexpr dword_ptr_t(x86_register reg_address) noexcept;

		x86_register data;
	};
}

#include <distant/assembly/impl/dword_ptr.hxx>
