#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <type_traits>

// enum_operators.hpp provides constexpr flag operators for use with strongly typed enums

#define DEFINE_CONSTEXPR_ENUM_FLAG_OPERATORS(ENUMTYPE) \
inline constexpr ENUMTYPE operator | (ENUMTYPE a, ENUMTYPE b) noexcept { return static_cast<ENUMTYPE>(static_cast<std::underlying_type_t<ENUMTYPE>>(a) | static_cast<std::underlying_type_t<ENUMTYPE>>(b)); } \
inline constexpr ENUMTYPE operator & (ENUMTYPE a, ENUMTYPE b) noexcept { return static_cast<ENUMTYPE>(static_cast<std::underlying_type_t<ENUMTYPE>>(a) & static_cast<std::underlying_type_t<ENUMTYPE>>(b)); } \
inline constexpr ENUMTYPE operator ^ (ENUMTYPE a, ENUMTYPE b) noexcept { return static_cast<ENUMTYPE>(static_cast<std::underlying_type_t<ENUMTYPE>>(a) ^ static_cast<std::underlying_type_t<ENUMTYPE>>(b)); } \
inline constexpr ENUMTYPE operator ~ (ENUMTYPE a)			  noexcept { return static_cast<ENUMTYPE>(~static_cast<std::underlying_type_t<ENUMTYPE>>(a)); } 
//inline ENUMTYPE& operator |= (ENUMTYPE &a, ENUMTYPE b) { return static_cast<ENUMTYPE>(((std::underlying_type_t<ENUMTYPE> &)a) |= ((std::underlying_type_t<ENUMTYPE>)b)); } \
//inline ENUMTYPE& operator &= (ENUMTYPE &a, ENUMTYPE b) { return static_cast<ENUMTYPE>(((std::underlying_type_t<ENUMTYPE> &)a) &= ((std::underlying_type_t<ENUMTYPE>)b)); } \
//inline ENUMTYPE& operator ^= (ENUMTYPE &a, ENUMTYPE b) { return static_cast<ENUMTYPE>(((std::underlying_type_t<ENUMTYPE> &)a) ^= ((std::underlying_type_t<ENUMTYPE>)b)); }
