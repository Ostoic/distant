#pragma once

#include <distant/concepts/boolean_validator.hpp>

namespace distant::concepts
{
	template <class Derived>
	struct handleable
		: public boolean_validator<handleable<Derived>>
	{
		bool valid() const noexcept
		{ return static_cast<const Derived*>(this)->handle_.valid(); }

		const auto& handle() const noexcept
		{ return static_cast<const Derived*>(this)->handle_; }

		auto& handle() noexcept
		{ return static_cast<Derived*>(this)->handle_; }
	};
}
