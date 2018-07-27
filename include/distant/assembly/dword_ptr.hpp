// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/memory/address.hpp>
#include <distant/assembly/registers.hpp>

namespace distant::assembly
{
	template <class T>
	struct dword_ptr_t
	{
		explicit constexpr dword_ptr_t(T t) noexcept
			: data(t)
		{}

		x86_register data;
	};

	template <class T>
	constexpr auto dword_ptr(T t) noexcept
	{
		return dword_ptr_t<T>{t};
	}
}

#include "impl/dword_ptr.hxx"