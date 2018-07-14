// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

namespace distant::utility {
	
	template <class Derived>
	class static_polymorphic
	{
	public:
		const Derived& derived() const
		{
			// Downcast (const) to the derived type.
			return *static_cast<const Derived*>(this);
		}

		Derived& derived()
		{
			// Downcast to the derived type.
			return *static_cast<Derived*>(this);
		}

	protected:
		// Protected keyword allows the dervied destructor to call this one
		// Disables destruction of derived type via the base
		~static_polymorphic() = default;
	};

} // end namespace distant::utility