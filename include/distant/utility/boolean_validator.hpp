// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

namespace distant::utility 
{
	template <typename Derived>
	class boolean_validator
	{
	public:
		explicit operator bool() const noexcept
		{ return static_cast<const Derived*>(this)->valid(); }
	};

} // namespace distant::utility
