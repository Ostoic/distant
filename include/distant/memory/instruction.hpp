#pragma once

#include <distant\memory\opcode.hpp>

namespace distant::memory
{
	template <std::size_t, std::size_t>
	class assembler;

	template <std::size_t AssemblerSize, std::size_t AssemblerCount>
	class instruction
	{
	private:
		using index_t = std::size_t;
		using length_t = std::size_t;

	public:
		//using iterator = assembler.

	public:
		constexpr instruction(const assembler<AssemblerSize, AssemblerCount>&, const std::pair<index_t, length_t>& ptr) noexcept;

		constexpr distant::byte operator[](std::size_t index) const noexcept;

		constexpr std::size_t size() const noexcept;

	private:
		const assembler<AssemblerSize, AssemblerCount>& assembler_;
		std::pair<index_t, length_t> ptr_;
	};
}

// Implementation:
#include <distant\memory\impl\instruction.hxx>