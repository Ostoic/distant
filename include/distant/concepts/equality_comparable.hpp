#pragma once

namespace distant::concepts
{
	template <class ComparableType>
	struct equality_comparable
	{
	protected:
		equality_comparable() = default;
	};

	template <class ComparableType>
	bool operator==(const equality_comparable<ComparableType>& lhs, const equality_comparable<ComparableType>& rhs) noexcept
	{
		return static_cast<const ComparableType&>(lhs).equals(static_cast<const ComparableType&>(rhs));
	}

	template <class ComparableType>
	bool operator!=(const equality_comparable<ComparableType>& lhs, const equality_comparable<ComparableType>& rhs) noexcept
	{
		return !(lhs == rhs);
	}
}
