#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <distant\error\gle.hpp>

#include <distant\utility\type_traits.hpp>
#include <distant\handle.hpp>
#include <distant\detail\handle_service.hpp>

namespace distant::kernel {

	/// Base class for kernel objects
	class object : public error::gle
	{
	public:
		// Go to MSDN for a list of kernel objects
		// XXX Provide link of objects

		using error_type  = object_traits<object>::error_type;
		using handle_type = object_traits<object>::handle_type;

	public:
		/*************************************/
		/** Windows object status functions **/
		/*************************************/
		/// Bivariant type cast for kernel objects
		template <typename other_t>
		const handle<other_t>& get_handle() const;
		
		/*********************************/
		/** Windows Object constructors **/
		/*********************************/
		/// Invalid handle default constructor
		constexpr object();

		/// Bivarient move constructor
		template <typename other_t>
		explicit object(handle<other_t>&& h);

		/// Move constructible
		object(object&& other);

		/// Move assignable
		object& operator =(object&& other);
 
	protected:
		/// Check if the process handle is valid
		bool valid() const;

		/// Close the underlying handle
		void close_object();

	protected:
		handle_type m_handle;
	};

} // end namespace distant::kernel

#include <distant\kernel\detail\object.hxx>

