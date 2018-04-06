// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

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
		// Make the default ctor constexpr
		constexpr Literal() = default;

	private:
		// Remove any operators that don't make sense to use with literals
		// Can't dynamically allocate or delete a literal (no pointers)
		void* operator new (std::size_t) = delete;
		void  operator delete(void*) = delete;
		
		// Address-of operator is forbiddien
		void  operator &() = delete;

		// Copy assignment is forbidden
		void  operator =(const Literal&) = delete;
	};

	// This allows the defined literals to be compared with a related literal
	template <
		typename T, 
		typename L/*,
		typename = std::enable_if_t<is_literal_type<T>::value>*/>
	inline constexpr bool operator==(const T& lhs, const Literal<L>& rhs)
	{
		constexpr L literal;

		// T possibly does not support constexpr construction, so we cannot create the temp constexpr 
		T temp(literal);

		return temp == lhs;
	}

	// Literal on left, type on right
	template <typename L, typename T>
	inline constexpr bool operator==(const Literal<L>& lhs, const T& rhs) { return operator==(rhs, lhs); }

	// Type on left, literal on right
	template <typename T, typename L>
	inline constexpr bool operator!=(const Literal<L>& lhs, const T& rhs) { return !operator==(lhs, rhs); }

	// Literal on right, type on left
	template <typename T, typename L>
	inline constexpr bool operator!=(const T& lhs, const Literal<T>& rhs) { return !operator==(lhs, rhs); }
	
} // end namespace distant::utility