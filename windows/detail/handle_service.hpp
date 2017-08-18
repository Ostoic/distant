#pragma once

#include <distant\utility\static_downcast.hpp>
#include <distant\windows\handle.hpp>

namespace distant::windows::detail {

	// Attempting to provide covariant/normal type-safe cast between distant::handles.
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
		const windows::handle<other_t>& get_handle() const
		{
			static_assert(
				is_related<Object_Type, other_t>::value,
				"Handle types are unrelated.");

			return *reinterpret_cast<const windows::handle<other_t>*>(&self()->m_handle);
		}

		// Return local object's handle
		const windows::handle<Object_Type>& get_handle() const
		{
			return self()->m_handle;
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


} // end namespace distant::windows::detail