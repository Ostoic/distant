#pragma once

#include <boost\winapi\basic_types.hpp>

namespace distant
{
	using word = unsigned short;
	using dword = boost::winapi::DWORD_;
	using qword = long long;

	using uint = unsigned int;
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
}

#include <distant\impl\types.hxx>
