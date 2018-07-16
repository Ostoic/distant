#pragma once

#include <tuple>

namespace distant::memory
{
	namespace detail
	{
		template <std::size_t I, class Tuple>
		struct aligned_offset_impl
		{
			constexpr std::size_t operator()() const noexcept
			{
				using current_element_t = std::tuple_element_t<I, Tuple>;
				using previous_element_t = std::tuple_element_t<I - 1, Tuple>;

				constexpr auto accumulated_offset = aligned_offset_impl<I - 1, Tuple>{}();

				// Check if the offset and size of the previous element gives a correct alignment for the current element.
				constexpr auto alignment_remainder = (accumulated_offset + sizeof(previous_element_t)) % alignof(current_element_t);

				if (alignment_remainder == 0)
					return accumulated_offset + sizeof(previous_element_t);

				// Calculate the distance needed to be meet the alignment requirements of the current element.
				constexpr auto needed_distance = (alignof(current_element_t) - accumulated_offset) % alignof(current_element_t);

				// If the distance matches the alignment, just use the alignment itself (mod == 0 gives 0)
				if (needed_distance % alignof(current_element_t) == 0)
					return accumulated_offset + alignof(current_element_t);

				// In the last case add the needed distance to meet the next element's alignment requirements.
				return accumulated_offset + needed_distance;
			}
		};

		template <class Tuple>
		struct aligned_offset_impl<0, Tuple>
		{
			constexpr std::size_t operator()() const noexcept
			{
				return 0;
			}
		};
	}

	/// @brief Calculate the aligned offset of an element in the given tuple as if it were put into a structure.
	/// @tparam I the index of the element in the tuple.
	/// @tparam Tuple the tuple to consider.
	/// @return the aligned offset
	/// @note The aligned offset of the \a I th element in \a Tuple depends on the previous \a I - 1 aligned offsets.
	template <std::size_t I, class Tuple>
	constexpr std::size_t aligned_offset() noexcept
	{
		return detail::aligned_offset_impl<I, Tuple>{}();
	}

	//template <class >

} // namespace distant::memory