#pragma once

namespace distant::concepts
{
	template <typename Derived>
	class boolean_validator
	{
	public:
		explicit operator bool() const noexcept
		{
			return static_cast<const Derived*>(this)->valid();
		}

	protected:
		boolean_validator() = default;
	};
}
