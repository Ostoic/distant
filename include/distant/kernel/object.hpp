#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <distant\error\windows_error.hpp>

#include <distant\utility\type_traits.hpp>
#include <distant\handle.hpp>
#include <distant\detail\handle_service.hpp>

namespace distant::kernel {

	/// Base class for kernel objects
	class object
	{
	public:
		using error_type  = object_traits<object>::error_type;
		using handle_type = object_traits<object>::handle_type;

	public:
		/// Bivariant type cast for kernel objects
		virtual const handle<object>& get_handle() const noexcept;
		
		/// Invalid handle default constructor
		object() noexcept;

		/// Bivariant handle move constructor
		template <typename other_t>	
		explicit object(handle<other_t>&& h) noexcept;

		/// Move constructible
		object(object&& other) noexcept;

		/// Move assignable
		object& operator =(object&& other) noexcept;

		/// Test if the object is valid or not
		/// \return true if the object is valid, false otherwise.
		virtual operator bool() const noexcept;

		/// Check if the process handle is valid
		virtual bool valid() const noexcept;

		virtual ~object() = default;

	protected:
		handle_type m_handle;
		mutable distant::error::windows_error m_last_error;
	};

} // end namespace distant::kernel

#include <distant\kernel\detail\object.hxx>

