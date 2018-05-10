// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <boost/winapi/basic_types.hpp>

#include <distant/detail/config.hpp>
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

	using index_t = unsigned int;

	template <typename Scalar, typename = std::enable_if_t<std::is_scalar<Scalar>::value>>
	constexpr byte get_byte(index_t index, Scalar scalar) noexcept;

	template <std::size_t N>
	constexpr byte get_byte(distant::word bytes) noexcept;

	template <std::size_t N>
	constexpr byte get_byte(distant::dword bytes) noexcept;

	template <std::size_t N>
	constexpr byte get_byte(distant::qword bytes) noexcept;

	template <typename Scalar>
	constexpr Scalar reverse_bytes(Scalar scalar) noexcept;

	template <typename... Bytes, typename = std::enable_if_t<sizeof...(Bytes) <= sizeof(distant::word)>>
	constexpr word make_word(Bytes&&... bytes) noexcept;

	template <typename... Bytes, typename = std::enable_if_t<sizeof...(Bytes) <= sizeof(distant::dword)>>
	constexpr dword make_dword(Bytes&&... bytes) noexcept;

	template <typename... Bytes, typename = std::enable_if_t<sizeof...(Bytes) <= sizeof(distant::qword)>>
	constexpr qword make_qword(Bytes&&... bytes) noexcept;

	template <typename Integer, typename... Bytes>
	constexpr Integer make_integer(Bytes&&... bytes) noexcept;
}

#include <distant/impl/types.hxx>
