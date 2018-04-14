// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once
#include "../dword_ptr.hpp"

namespace distant::assembly
{
//class dword_ptr
//public:
	template<typename T, typename S>
	constexpr dword_ptr_t<T, S>::dword_ptr_t(T data) noexcept
		: data(data) {}
}
