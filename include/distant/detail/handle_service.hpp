#pragma once

#include <distant\utility\static_downcast.hpp>
#include <distant\handle.hpp>

namespace distant::detail {

	// Provide covariant/normal type-safe cast between distant::handles.
	// The host class would inherit from handle_service, though multiple levels of 
	// inheritance cause ambiguity in calls to get_handle.
	template <class Object_Type>
	class handle_service : private utility::static_downcast<Object_Type>
	{
	public:
		// Return local object's handle.
		// Performs a covariant cast between handle types 
		// if the contained object_type in each handle are
		// compatible.
		template <typename other_t>
		const handle<other_t>& get_handle() const
		{
			static_assert(
				is_related<Object_Type, other_t>::value,
				"Handle types are unrelated.");

			return *reinterpret_cast<const handle<other_t>*>(&static_cast<const Object_Type*>(this)->m_handle);
		}

		// Return local object's handle
		const handle<Object_Type>& get_handle() const
		{
			return static_cast<const Object_Type*>(this)->m_handle;
		}

		bool valid() const
		{
			return self()->m_handle.valid();
		}

		explicit operator bool() const
		{
			return this->valid();
		}
	};


} // end namespace distant::detail
