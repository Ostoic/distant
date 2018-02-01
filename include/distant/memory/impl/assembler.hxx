#pragma once
#include <distant\memory\assembler.hpp>

namespace distant::memory
{
//class assembler:
//{ctor}:
	template <std::size_t C, std::size_t I>
	constexpr assembler<C, I>::assembler(
		const std::array<distant::byte, C>& instructions, 
		const std::array<std::pair<std::size_t, std::size_t>, I>& instruction_ptrs) noexcept
			: bytes_(instructions)
			, instruction_ptrs_(instruction_ptrs) {}


//interface:
	template <std::size_t S, std::size_t C>
	constexpr typename assembler<S, C>::iterator
	assembler<S, C>::begin() const
	{
		return iterator{*this, 0};
	}

	template <std::size_t S, std::size_t C>
	constexpr typename assembler<S, C>::iterator
	assembler<S, C>::end() const
	{
		return iterator{*this, C};
	}

//free:
	template <std::size_t F, std::size_t FI, std::size_t S>
	constexpr assembler<F + S, FI + 1> operator+(const assembler<F, FI>& first, const assembler<S, 1>& second) noexcept
	{
		return {
			utility::append(first.bytes_, second.bytes_),
			utility::append(first.instruction_ptrs_, utility::make_array(std::make_pair(F, S)))
		};
	}

	template <typename... Bytes, typename>
	constexpr auto make_instruction(opcode op, Bytes&&... bytes) noexcept
	{
		using index = std::size_t;
		using length = std::size_t;

		return assembler<sizeof...(Bytes)+1, 1>{
			std::array<distant::byte, sizeof...(Bytes)+1>
				{static_cast<distant::byte>(op), static_cast<distant::byte>(bytes)...},

			std::array<std::pair<index, length>, 1>
				{std::make_pair(0, sizeof...(Bytes)+1)}
		};
	}

	constexpr auto make_instruction(opcode op) noexcept
	{
		using index = std::size_t;
		using length = std::size_t;

		return assembler<1, 1> {
			std::array<distant::byte, 1>
				{static_cast<distant::byte>(op)},

			std::array<std::pair<index, length>, 1>
				{std::make_pair(0, 1)}
		};
	}

}
