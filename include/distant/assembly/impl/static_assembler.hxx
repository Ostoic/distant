// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once
#include <distant/assembly/static_assembler.hpp>

#include <distant/types.hpp>

namespace distant::memory
{
//class assembler:
//{ctor}:
	template <std::size_t C, std::size_t I>
	constexpr static_assembler<C, I>::static_assembler(
		const std::array<distant::byte, C>& instructions, 
		const std::array<std::pair<std::size_t, std::size_t>, I>& instruction_ptrs) noexcept
			: bytes_(instructions)
			, instruction_ptrs_(instruction_ptrs) {}


//interface:
	template <std::size_t S, std::size_t C>
	constexpr typename static_assembler<S, C>::iterator
	static_assembler<S, C>::begin() const
	{
		return iterator{*this, 0};
	}

	template <std::size_t S, std::size_t C>
	constexpr typename static_assembler<S, C>::iterator
	static_assembler<S, C>::end() const
	{
		return iterator{*this, C};
	}

//free:
	template <std::size_t F, std::size_t FI, std::size_t S>
	constexpr static_assembler<F + S, FI + 1> operator+(const static_assembler<F, FI>& first, const static_assembler<S, 1>& second) noexcept
	{
		return {
			meta::append(first.bytes_, second.bytes_),
			meta::append(first.instruction_ptrs_, meta::make_array(std::make_pair(F, S)))
		};
	}

	template <typename... Bytes, typename>
	constexpr auto make_instruction(opcode op, Bytes&&... bytes) noexcept
	{
		using under = std::underlying_type_t<opcode>;
		using index = std::size_t;
		using length = std::size_t;
		using distant::get_byte;

		// The opcode can be represented by a byte
		return static_assembler<sizeof...(Bytes) + sizeof(opcode), 1> {
			std::array<distant::byte, sizeof...(Bytes) + 2> {
				get_byte<0>(static_cast<under>(op)),
				get_byte<1>(static_cast<under>(op)),
				static_cast<distant::byte>(bytes)...
			},

			std::array<std::pair<index, length>, 1> {
				std::make_pair(0, sizeof...(Bytes) + 2)
			}
		};
	}

	constexpr auto make_instruction(opcode op) noexcept
	{
		using index = std::size_t;
		using length = std::size_t;
		using under = std::underlying_type_t<opcode>;
		using distant::get_byte;

		return static_assembler<sizeof(opcode), 1> {
			std::array<distant::byte, 2> {
				static_cast<distant::byte>(get_byte<0>(static_cast<under>(op))),
				static_cast<distant::byte>(get_byte<1>(static_cast<under>(op)))
			},

			std::array<std::pair<index, length>, 1> {
				std::make_pair(0, 2)
			}
		};
	}

	template <std::size_t N, std::size_t Size, std::size_t InstrCount>
	constexpr auto get(const static_assembler<Size, InstrCount>& a) noexcept
	{
		return *(a.begin() + N);
	}
}
