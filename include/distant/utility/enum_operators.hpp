// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

// enum_operators.hpp provides constexpr flag operators for use with strongly typed enums

#define DEFINE_CONSTEXPR_ENUM_FLAG_OPERATORS(ENUMTYPE) \
	constexpr ENUMTYPE operator | (ENUMTYPE a, ENUMTYPE b) noexcept { return static_cast<ENUMTYPE>(static_cast<std::underlying_type_t<ENUMTYPE>>(a) | static_cast<std::underlying_type_t<ENUMTYPE>>(b)); } \
	constexpr ENUMTYPE operator & (ENUMTYPE a, ENUMTYPE b) noexcept { return static_cast<ENUMTYPE>(static_cast<std::underlying_type_t<ENUMTYPE>>(a) & static_cast<std::underlying_type_t<ENUMTYPE>>(b)); } \
	constexpr ENUMTYPE operator ^ (ENUMTYPE a, ENUMTYPE b) noexcept { return static_cast<ENUMTYPE>(static_cast<std::underlying_type_t<ENUMTYPE>>(a) ^ static_cast<std::underlying_type_t<ENUMTYPE>>(b)); } \
	constexpr ENUMTYPE operator ~ (ENUMTYPE a)			  noexcept { return static_cast<ENUMTYPE>(~static_cast<std::underlying_type_t<ENUMTYPE>>(a)); } 

#define DEFINE_CONSTEXPR_ENUM_FLAG_PARTIAL_ORDER(ENUMTYPE) \
	constexpr bool operator<=(const ENUMTYPE lhs, const ENUMTYPE rhs) noexcept { return (lhs & rhs) == lhs; }\
	constexpr bool operator>=(const ENUMTYPE lhs, const ENUMTYPE rhs) noexcept { return (lhs & rhs) == rhs; }\
	constexpr bool operator< (const ENUMTYPE lhs, const ENUMTYPE rhs) noexcept { return operator<=(lhs, rhs) && lhs != rhs; }\
	constexpr bool operator> (const ENUMTYPE lhs, const ENUMTYPE rhs) noexcept { return operator>=(lhs, rhs) && lhs != rhs; }

/**
 * reflexive: vm_write & vm_write = vm_write
 * antisymmetric: vm_write <=, >= vm_write means vm_write & vm_write == vm_write, vm_write
 */