#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <distant\windows\handle.hpp>
#include <distant\windows\error\gle.hpp>

#include <distant\windows\detail\handle_service.hpp>

#include <distant\utility\type_traits.hpp>

//#include <distant\detail\fwd.h>

namespace distant::windows::kernel {

	// Go to MSDN for more information
	// Base kernel object for windows
	// Default: encode self type into handle_type
	// Specified: encodes specified type into handle_type
	class object : 
		public error::gle
		//public windows::detail::handle_service<object>
	{
	public:
		using error_type  = object_traits<object>::error_type;
		using handle_type = object_traits<object>::handle_type;

	public:
		/*************************************/
		/** Windows object status functions **/
		/*************************************/
		// Implicitly cast object to handle_type
		template <typename other_t>
		const windows::handle<other_t>& get_handle() const;
		
		template <typename other_t>
		explicit operator const windows::handle<other_t>&() const;

		/*********************************/
		/** Windows Object constructors **/
		/*********************************/
		// Invalid handle default constructor
		constexpr object();

		template <typename other_t>
		explicit object(windows::handle<other_t>&& h);

		// Move constructible/assignable.
		object(object&& other);

		object& operator =(object&& other);
 
		// Calls handle destructor
		~object() {}

		friend bool operator ==(const kernel::object&, const kernel::object&);
		friend bool operator !=(const kernel::object&, const kernel::object&);

	protected:
		// Determine if the object handle is valid
		bool weakly_valid() const;

		void close_object();

	protected:
		handle_type m_handle;
	};

} // end namespace distant::windows::kernel

#include <distant\windows\kernel\detail\object.inl>

