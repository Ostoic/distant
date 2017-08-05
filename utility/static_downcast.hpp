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