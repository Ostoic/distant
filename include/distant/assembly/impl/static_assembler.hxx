// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once
#include <distant/assembly/static_assembler.hpp>

#include <distant/types.hpp>

namespace distant::assembly
{
//class assembler:
//{ctor}:
	template <std::size_t C, std::size_t I>
	constexpr static_assembler<C, I>::static_assembler(
		std::array<byte, C> bytes,
		std::array<index_t, I> instruction_ptrs) noexcept
			: bytes(std::move(bytes))
			, instruction_ptrs_(std::move(instruction_ptrs)) 
	{}

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

	template <std::size_t S, std::size_t C>
	constexpr auto static_assembler<S, C>::operator[](const index_t index) const noexcept
	{
		return static_instruction<S, C>(*this, index);
	}

//free:
	template <std::size_t F, std::size_t FI, std::size_t S>
	constexpr static_assembler<F + S, FI + 1> 
		operator+(const static_assembler<F, FI>& first, const static_assembler<S, 1>& second) noexcept
	{
		return {
			utility::meta::append(first.bytes, second.bytes),
			utility::meta::append(first.instruction_ptrs_, {F})
		};
	}

	template <typename... Bytes, typename>
	constexpr auto make_instruction(opcode op, Bytes&&... bytes) noexcept
	{
		using under = std::underlying_type_t<opcode>;
		using utility::meta::make_array;
		using distant::get_byte;

		const auto bytes_array = make_array(
			get_byte<0>(static_cast<under>(op)),
			get_byte<1>(static_cast<under>(op)),
			static_cast<byte>(bytes)...
		);

		// The opcode is represented by bytes_array.
		return static_assembler<sizeof...(Bytes) + sizeof(opcode), 1>(bytes_array, {0});
	}

	constexpr auto make_instruction(opcode op) noexcept
	{
		using under = std::underlying_type_t<opcode>;
		using utility::meta::make_array;
		using distant::get_byte;

		const auto bytes_array = make_array(
			static_cast<byte>(get_byte<0>(static_cast<under>(op))),
			static_cast<byte>(get_byte<1>(static_cast<under>(op)))
		);

		return static_assembler<sizeof(opcode), 1>(bytes_array, {0});
	}

	template <std::size_t N, std::size_t Size, std::size_t InstructionCount>
	constexpr auto get_instruction(const static_assembler<Size, InstructionCount>& assembler) noexcept
	{
		return *(assembler.begin() + N);
	}
}
