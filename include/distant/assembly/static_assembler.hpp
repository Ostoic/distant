#pragma once

#include <distant/types.hpp>
#include <distant/utility/meta/algorithm.hpp>

#include <distant/assembly/static_instruction.hpp>
#include <distant/assembly/static_assembler_iterator.hpp>

namespace distant::memory
{
	template <std::size_t Size = 0, std::size_t InstrCount = 0>
	class static_assembler
	{
	public:
		using iterator = static_assembler_iterator<Size, InstrCount>;
		using const_iterator = iterator;

	public: // {ctor}
		constexpr static_assembler() = default;
		constexpr static_assembler(const static_assembler& other) noexcept = default;

	public: // interface
		constexpr iterator begin() const;
		constexpr iterator end() const;

	public: // operators
		template <typename... Bytes,
		          typename = std::enable_if_t<
			          std::is_convertible_v<std::common_type_t<Bytes...>, byte>
		          >
		>
		friend constexpr auto make_instruction(opcode op, Bytes&&... bytes) noexcept;

		friend constexpr auto make_instruction(opcode op) noexcept;

		// TODO: Extend this to operator that can take a second argument with an arbitrary number of instructions.
		// ***** To do this, we need to transform the instruction_ptrs indices + InstrCount.
		template <std::size_t First, std::size_t FirstCount, std::size_t Second>
		friend constexpr static_assembler<First + Second, FirstCount + 1>
		operator+(const static_assembler<First, FirstCount>& first, const static_assembler<Second, 1>& second) noexcept;

	private:
		using index = std::size_t;
		using instruction_length = std::size_t;

		constexpr static_assembler(
			const std::array<byte, Size>& bytes,
			const std::array<std::pair<index, instruction_length>, InstrCount>& instruction_ptrs) noexcept;
	private:
		friend class static_instruction<Size, InstrCount>;
		friend class static_assembler_iterator<Size, InstrCount>;

		std::array<byte, Size> bytes_;

		// instruction_ptrs_ holds indices to where each instruction in bytes_ is located,
		// along with the length of the instruction.
		std::array<std::pair<index, instruction_length>, InstrCount> instruction_ptrs_;
	};

	template <typename... Bytes,
	          typename = std::enable_if_t<
		          std::is_convertible_v<std::common_type_t<Bytes...>, byte>
	          >
	>
	constexpr auto make_instruction(opcode op, Bytes&&... bytes) noexcept;

	constexpr auto make_instruction(opcode op) noexcept;

	/**********************************************************************************************/ /**
	 * @brief Access the nth instruction of the given assembler.
	 *
	 * @tparam N The index of the assembler to access.   
	 * @tparam Size The size of the assembler.
	 * @tparam InstrCount The number of instructions that the assembler can hold.
	 * @param a An assembler of the given size and instruction count.
	 *
	 * @return An instruction of the corresponding size.
	 **************************************************************************************************/
	template <std::size_t N, std::size_t Size, std::size_t InstrCount>
	constexpr auto get(const static_assembler<Size, InstrCount>& a) noexcept;
}

// Implementation:
#include <distant/assembly/impl/static_assembler.hxx>
