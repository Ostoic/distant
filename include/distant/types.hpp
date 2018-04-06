// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <boost/winapi/basic_types.hpp>

#include <cstddef>

namespace distant
{
	using word = unsigned short;
	using dword = boost::winapi::DWORD_;
	using qword = long long;

	using uint = unsigned int;

	// Todo: C++17 std::byte version
	//enum class byte : unsigned char {};
	using byte = unsigned char;

	template <std::size_t N>
	constexpr distant::byte get(distant::word bytes) noexcept;

	template <std::size_t N>
	constexpr distant::byte get(distant::dword bytes) noexcept;

	template <std::size_t N>
	constexpr distant::byte get(distant::qword bytes) noexcept;

	template <typename... Bytes, typename = std::enable_if_t<sizeof...(Bytes) <= sizeof(distant::word)>>
	constexpr distant::word make_word(Bytes&&... bytes) noexcept;

	template <typename... Bytes, typename = std::enable_if_t<sizeof...(Bytes) <= sizeof(distant::dword)>>
	constexpr distant::dword make_dword(Bytes&&... bytes) noexcept;

	template <typename... Bytes, typename = std::enable_if_t<sizeof...(Bytes) <= sizeof(distant::qword)>>
	constexpr distant::qword make_qword(Bytes&&... bytes) noexcept;

	template <typename Integer, typename... Bytes>
	constexpr Integer make_integer(Bytes&&... bytes) noexcept;

}

#include <distant/impl/types.hxx>
