#pragma once
#include <distant\types.hpp>

namespace distant
{
	template <std::size_t N>
	inline constexpr distant::byte get(distant::word bytes) noexcept
	{
		if constexpr (N >= sizeof(distant::word))
			static_assert(false, "[distant::get<word>] Byte index out of range");

		return (bytes >> (8 * N)) & 0xff;
	}

	template <std::size_t N>
	inline constexpr distant::byte get(distant::dword bytes) noexcept
	{
		if constexpr (N >= sizeof(distant::dword))
			static_assert(false, "[distant::get<dword>] Byte index out of range");

		return (bytes >> (8 * N)) & 0xff;
	}

	template <std::size_t N>
	inline constexpr distant::byte get(distant::qword bytes) noexcept
	{
		if constexpr (N >= sizeof(distant::qword))
			static_assert(false, "[distant::get<qword>] Byte index out of range");

		return (bytes >> (8 * N)) & 0xff;
	}

	namespace detail
	{
		template <typename Return, std::size_t S>
		inline constexpr auto make_impl(const std::array<distant::byte, S>& array) noexcept
		{
			Return result = array[0];
			for (std::size_t i = 1; i < S; ++i)
				result |= (array[i] << 8 * i);

			return result;
		}
	}

	template <typename... Bytes, typename>
	inline constexpr distant::word make_word(Bytes&&... bytes) noexcept
	{
		using result_t = distant::word;
		if constexpr (!std::is_convertible<std::common_type_t<Bytes...>, distant::byte>::value)
			static_assert(false, "[distant::make_word] Type of bytes must be convertible to distant::byte");

		return detail::make_impl<distant::word>(
			meta::make_array((static_cast<distant::byte>(std::forward<Bytes>(bytes)))...)
		);
	}

	template <typename... Bytes, typename>
	inline constexpr distant::dword make_dword(Bytes&&... bytes) noexcept
	{
		using result_t = distant::dword;
		if constexpr (!std::is_convertible<std::common_type_t<Bytes...>, distant::byte>::value)
			static_assert(false, "[distant::make_dword] Type of bytes must be convertible to distant::byte");

		return detail::make_impl<result_t>(
			meta::make_array((static_cast<distant::byte>(std::forward<Bytes>(bytes)))...)
		);
	}

	template <typename... Bytes, typename>
	inline constexpr distant::qword make_qword(Bytes&&... bytes) noexcept
	{
		using result_t = distant::qword;
		if constexpr (!std::is_convertible<std::common_type_t<Bytes...>, distant::byte>::value)
			static_assert(false, "[distant::make_qword] Type of bytes must be convertible to distant::byte");

		return detail::make_impl<result_t>(
			meta::make_array((static_cast<distant::byte>(std::forward<Bytes>(bytes)))...)
		);
	}
}
