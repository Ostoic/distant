#pragma once

#include <Windows.h>

#include <type_traits>

#define DEFINE_CONSTEXPR_ENUM_FLAG_OPERATORS(ENUMTYPE) \
inline constexpr ENUMTYPE  operator | (ENUMTYPE a, ENUMTYPE b) { return static_cast<ENUMTYPE>(static_cast<std::underlying_type_t<ENUMTYPE>>(a) | static_cast<std::underlying_type_t<ENUMTYPE>>(b)); } \
inline constexpr ENUMTYPE  operator & (ENUMTYPE a, ENUMTYPE b) { return static_cast<ENUMTYPE>(static_cast<std::underlying_type_t<ENUMTYPE>>(a) & static_cast<std::underlying_type_t<ENUMTYPE>>(b)); } \
inline constexpr ENUMTYPE  operator ^ (ENUMTYPE a, ENUMTYPE b) { return static_cast<ENUMTYPE>(static_cast<std::underlying_type_t<ENUMTYPE>>(a) ^ static_cast<std::underlying_type_t<ENUMTYPE>>(b)); } \
inline constexpr ENUMTYPE  operator ~ (ENUMTYPE a)			   { return static_cast<ENUMTYPE>(~static_cast<std::underlying_type_t<ENUMTYPE>>(a)); } 
//inline ENUMTYPE& operator |= (ENUMTYPE &a, ENUMTYPE b) { return static_cast<ENUMTYPE>(((std::underlying_type_t<ENUMTYPE> &)a) |= ((std::underlying_type_t<ENUMTYPE>)b)); } \
//inline ENUMTYPE& operator &= (ENUMTYPE &a, ENUMTYPE b) { return static_cast<ENUMTYPE>(((std::underlying_type_t<ENUMTYPE> &)a) &= ((std::underlying_type_t<ENUMTYPE>)b)); } \
//inline ENUMTYPE& operator ^= (ENUMTYPE &a, ENUMTYPE b) { return static_cast<ENUMTYPE>(((std::underlying_type_t<ENUMTYPE> &)a) ^= ((std::underlying_type_t<ENUMTYPE>)b)); }