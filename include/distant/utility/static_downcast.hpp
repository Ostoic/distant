// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

namespace distant::utility {
	
	template <class derived>
	class static_downcast
	{
	public:
		const derived* self() const
		{
			// Downcast (const) to the derived type.
			return static_cast<const derived*>(this);
		}

		derived* self()
		{
			// Downcast to the derived type.
			return static_cast<derived*>(this);
		}

	protected:
		// Protected keyword allows the dervied destructor to call this one
		// Disables destruction of derived type via the base
		~static_downcast() = default;
	};

} // end namespace distant::utility