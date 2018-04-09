// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

namespace distant::utility {

	template <typename LiteralT>
	class literal
	{
	public:
		// Make the default ctor constexpr
		constexpr literal() = default;

		// Remove any operators that don't make sense to use with literals
		// Can't dynamically allocate or delete a literal (no pointers)
		void* operator new (std::size_t) = delete;
		void  operator delete(void*) = delete;

		// Address-of operator is forbiddien
		void  operator &() = delete;

		// Copy assignment is forbidden
		void  operator =(const literal&) = delete;
	};

	// This allows the defined literals to be compared with a related literal
	template <
		typename T, 
		typename L/*,
		typename = std::enable_if_t<is_literal_type<T>::value>*/>
	constexpr bool operator==(const T& lhs, const literal<L>& rhs)
	{
		constexpr L literal;

		// T possibly does not support constexpr construction, so we cannot create the temp constexpr 
		T temp(literal);

		return temp == lhs;
	}

	// literal on left, type on right
	template <typename L, typename T>
	constexpr bool operator==(const literal<L>& lhs, const T& rhs) { return operator==(rhs, lhs); }

	// Type on left, literal on right
	template <typename T, typename L>
	constexpr bool operator!=(const literal<L>& lhs, const T& rhs) { return !operator==(lhs, rhs); }

	// literal on right, type on left
	template <typename T, typename L>
	constexpr bool operator!=(const T& lhs, const literal<T>& rhs) { return !operator==(lhs, rhs); }
	
} // end namespace distant::utility