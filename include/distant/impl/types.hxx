// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once
#include <distant/types.hpp>

#include <distant/utility/meta/algorithm.hpp>

namespace distant
{
	template <std::size_t N>
	constexpr byte get_byte(const word bytes) noexcept
	{
		static_assert(N < sizeof(word), "[distant::get_byte<word>] Byte index out of range");

		return (bytes >> (8 * N)) & 0xff;
	}

	template <std::size_t N>
	constexpr byte get_byte(const dword bytes) noexcept
	{
		static_assert(N < sizeof(word), "[distant::get_byte<dword>] Byte index out of range");

		return (bytes >> (8 * N)) & 0xff;
	}

	template <std::size_t N>
	constexpr byte get_byte(const qword bytes) noexcept
	{
		static_assert(N < sizeof(word), "[distant::get_byte<qword>] Byte index out of range");

		return (bytes >> (8 * N)) & 0xff;
	}

	namespace detail
	{
		template <typename Return, std::size_t S>
		constexpr auto make_impl(const std::array<byte, S>& array) noexcept
		{
			Return result = array[0];
			for (std::size_t i = 1; i < S; ++i)
				result |= (array[i] << 8 * i);

			return result;
		}
	}

	template <typename... Bytes, typename>
	constexpr word make_word(Bytes&&... bytes) noexcept
	{
		static_assert(
			std::is_convertible<std::common_type_t<Bytes...>, byte>::value, 
			"[distant::make_word] Type of bytes must be convertible to distant::byte"
		);

		using result_t = word;

		return detail::make_impl<word>(
			utility::meta::make_array(static_cast<byte>(std::forward<Bytes>(bytes))...)
		);
	}

	template <typename... Bytes, typename>
	constexpr dword make_dword(Bytes&&... bytes) noexcept
	{
		static_assert(
			std::is_convertible<std::common_type_t<Bytes...>, byte>::value, 
			"[distant::make_dword] Type of bytes must be convertible to distant::byte"
		);

		using result_t = dword;

		return detail::make_impl<result_t>(
			meta::make_array(static_cast<byte>(std::forward<Bytes>(bytes))...)
		);
	}

	template <typename... Bytes, typename>
	constexpr qword make_qword(Bytes&&... bytes) noexcept
	{
		static_assert(
			std::is_convertible<std::common_type_t<Bytes...>, byte>::value, 
			"[distant::make_qword] Type of bytes must be convertible to distant::byte"
		);

		using result_t = qword;

		return detail::make_impl<result_t>(
			utility::meta::make_array(static_cast<byte>(std::forward<Bytes>(bytes))...)
		);
	}

	template <typename Integer, typename... Bytes>
	constexpr Integer make_integer(Bytes&&... bytes) noexcept
	{
		static_assert(
			std::is_convertible<std::common_type_t<Bytes...>, Integer>::value, 
			"[distant::make_qword] Type of bytes must be convertible to distant::byte"
		);

		using result_t = Integer;

		return detail::make_impl<result_t>(
			utility::meta::make_array(static_cast<byte>(std::forward<Bytes>(bytes))...)
		);
	}
}
