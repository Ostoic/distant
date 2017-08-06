#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <utility>

namespace distant::utility {

	template <typename Unique_Literal_t>
	class Literal
	{
	public:
		// Require the default ctor for the subtype to be constexpr
		constexpr Literal() = default;

	private:
		void* operator new (std::size_t) = delete;
		void  operator delete(void*) = delete;
		
		void  operator &() = delete;
		void  operator =(const Literal&) = delete;
	};

	// This allows the defined literals to be compared with a related literal
	template <typename T, typename L>
	inline constexpr bool operator==(const T& lhs, const Literal<L>& rhs)
	{
		constexpr L literal;
		T temp(literal);

		return temp == lhs;
	}

	// Literal on left, type on right
	template <typename L, typename T>
	inline constexpr bool operator==(const Literal<L>& lhs, const T& rhs) { return operator==(rhs, lhs); }

	// Type on left, literal on right
	template <typename T, typename L>
	inline constexpr bool operator!=(const Literal<L>& lhs, const T& rhs) { return !operator==(lhs, rhs); }

	// Literal on left, type on right
	template <typename L, typename T>
	inline constexpr bool operator!=(const Literal<L>& lhs, const T& rhs) { return operator!=(rhs, lhs); }
	
} // end namespace distant::utility