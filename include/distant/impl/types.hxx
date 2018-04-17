// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once
#include <distant/types.hpp>

#include <distant/utility/meta/algorithm.hpp>

#include <boost/assert.hpp>

namespace distant
{
	template <typename Scalar, typename>
	constexpr byte get_byte(const index_t index, const Scalar scalar) noexcept
	{
		BOOST_ASSERT_MSG(index < sizeof(Scalar), "[distant::get_byte] Byte index out of range");

		return (scalar >> (8 * index)) & 0xff;
	}

	template <std::size_t N>
	constexpr byte get_byte(const word bytes) noexcept
	{
		static_assert(N < sizeof(word), "[distant::get_byte<word>] Byte index out of range");
		return get_byte(N, bytes);
	}

	template <std::size_t N>
	constexpr byte get_byte(const dword bytes) noexcept
	{
		static_assert(N < sizeof(word), "[distant::get_byte<dword>] Byte index out of range");
		return get_byte(N, bytes);
	}

	template <std::size_t N>
	constexpr byte get_byte(const qword bytes) noexcept
	{
		static_assert(N < sizeof(word), "[distant::get_byte<qword>] Byte index out of range");
		return get_byte(N, bytes);
	}

	namespace detail
	{
		template <typename Return, std::size_t S>
		constexpr auto make_impl(const std::array<byte, S>& array) noexcept
		{
			namespace meta = utility::meta;
			Return result = 0;

			index_t i = 0;
			meta::for_each_tuple(array, [&result, &i](const byte b)
			{
				result |= (b << 8 * i++);
			});

			//for (std::size_t i = 1; i < S; ++i)

			return result;
		}

		template <typename T>
		struct reverse_bytes_generator
		{
			const T& data;

			constexpr explicit reverse_bytes_generator(const T& d) : data(d) {}

			constexpr auto operator()(const index_t index) const noexcept
			{
				return get_byte(sizeof(T) - 1 - index, data);
			}
		};
	}


	template <typename Scalar>
	constexpr Scalar reverse_bytes(const Scalar scalar) noexcept
	{
		namespace meta = utility::meta;

		const auto generator = detail::reverse_bytes_generator<Scalar>(scalar);

		return detail::make_impl<Scalar>(meta::generate_array<sizeof(Scalar)>(generator));
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
			utility::meta::make_array(static_cast<byte>(std::forward<Bytes>(bytes))...)
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
