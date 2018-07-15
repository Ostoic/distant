#pragma once

namespace distant::concepts
{
	template <class DerivedComparable>
	struct equality_comparable
	{
	protected:
		equality_comparable() = default;
	};

	template <class DerivedComparable>
	constexpr bool operator==(const equality_comparable<DerivedComparable>& lhs, const equality_comparable<DerivedComparable>& rhs) noexcept
	{
		return static_cast<const DerivedComparable&>(lhs).equals(static_cast<const DerivedComparable&>(rhs));
	}

	template <class DerivedComparable>
	constexpr bool operator!=(const equality_comparable<DerivedComparable>& lhs, const equality_comparable<DerivedComparable>& rhs) noexcept
	{
		return !(lhs == rhs);
	}
}
