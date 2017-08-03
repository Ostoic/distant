#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <distant\windows\handle.hpp>
#include <distant\windows\error\gle.hpp>

#include <distant\type_traits.hpp>

//#include <distant\detail\fwd.h>

namespace distant {
namespace windows {
namespace kernel  {

	// Go to MSDN for more information
	// Base kernel object for windows
	// Default: encode self type into handle_type
	// Specified: encodes specified type into handle_type
	class object : private error::gle
	{
	public:
		using error_type  = object_traits<object>::error_type;
		using handle_type = object_traits<object>::handle_type;

	public:
		using gle::get_last_error;
		
		/*************************************/
		/** Windows object status functions **/
		/*************************************/
		// Implicitly cast object to handle_type
	public:
		template <typename other_t>
		const windows::handle<other_t>& get_handle() const 
		{ 
			static_assert(
				is_related<object, other_t>::value,
				"Handle types are not convertible.");

			return *reinterpret_cast<const windows::handle<other_t>*>(&m_handle);
		}
		
		template <typename other_t>
		explicit operator const windows::handle<other_t>&() const { return get_handle<other_t>(); }

		// Determine if the object handle is valid
		bool weakly_valid() const
		{ return m_handle.weakly_valid(); }

		/*********************************/
		/** Windows Object constructors **/
		/*********************************/
		// Invalid handle default constructor
		constexpr object() : m_handle(invalid_handle) {}

		template <typename other_t>
		explicit object(windows::handle<other_t>&& h) : 
			m_handle(std::move(h))
		{}

		object(object&& other) :
			object(std::move(other.m_handle))
		{}

		object& operator =(object&& other)
		{
			m_handle = std::move(other.m_handle);
			return *this;
		}
 
		// Calls handle destructor
		~object() {}

		friend bool operator ==(const kernel::object&, const kernel::object&);
		friend bool operator !=(const kernel::object&, const kernel::object&);

	protected:
		using gle::update_gle;

		void close_object()
		{ m_handle.close_handle(); }

	protected:
		handle_type m_handle;
	};
	
	inline bool operator ==(const kernel::object& lhs, const kernel::object& rhs)
	{
		return windows::operator==(lhs.m_handle, rhs.m_handle);
	}

	inline bool operator !=(const kernel::object& lhs, const kernel::object& rhs)
	{
		return windows::operator!=(lhs.m_handle, rhs.m_handle);
	}

} // end namespace kernel
} // end namespace windows
} // end namespace distant

