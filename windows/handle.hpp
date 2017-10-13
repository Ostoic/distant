#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

// For Windows SDK selection
#include <ntverp.h>

#include <distant\detail\attorney.hpp>

#include <distant\windows\access_rights.hpp>
#include <distant\windows\detail\handle_base.hpp>

namespace distant {
namespace windows {

	// Type-safe handle for windows objects
	template <typename Object_t> 
	class handle : public detail::handle_base 
	{
	private:
		using handle_base = detail::handle_base;

	public:
		using object_type = Object_t;

	public:
		constexpr explicit handle(native_type h, flag_type flags);
		constexpr explicit handle(native_type h);

		// invalid_handle literal ctor
		constexpr handle();
		constexpr handle(detail::invalid_t);

		// Bivariant move constructor
		template <typename other_t>
		handle(handle<other_t>&& other);
		
		// Bivariant move assignment
		template <typename other_t>
		handle& operator =(handle<other_t>&& other);
		
		// Close handle to windows object.
		// Handle must be weakly valid in order to close the handle.
		~handle() = default;

	private:	
		// Allow attorney to expose some implementation details
		// This is mainly for recreated winapi functions to pass
		// the underlying handle value into the winapi.
		template <typename>
		friend class distant::detail::attorney::to_handle;

		// Expose implementation to other handle types
		template <typename>
		friend class handle;

	public:
		template <typename T, typename U>
		friend constexpr bool operator ==(const handle<T>&, const handle<U>&);

		template <typename T, typename U>
		friend constexpr bool operator !=(const handle<T>&, const handle<U>&);
	};

} // end namespace windows

using windows::handle;
using windows::invalid_handle;

} // end namespace distant

#include <distant\windows\detail\handle.inl>