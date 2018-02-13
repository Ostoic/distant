#pragma once

#include <utility>

namespace distant::memory
{
	template <std::size_t, std::size_t>
	class static_assembler;

	template <std::size_t AssemblerSize, std::size_t AssemblerCount>
	class static_instruction
	{
	private:
		using index_t = std::size_t;
		using length_t = std::size_t;

	public:
		constexpr static_instruction(const static_assembler<AssemblerSize, AssemblerCount>&,
		                             const std::pair<index_t, length_t>& ptr) noexcept;

		constexpr byte operator[](std::size_t index) const noexcept;

		constexpr std::size_t size() const noexcept;

	private:
		const static_assembler<AssemblerSize, AssemblerCount>& assembler_;
		std::pair<index_t, length_t> ptr_;
	};
}

// Implementation:
#include <distant/assembly/impl/static_instruction.hxx>
