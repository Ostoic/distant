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
	template <typename Object_t> // Object type is statically encoded into handle
	class handle : public detail::handle_base // Implementation
	{
	private:
		using handle_base = detail::handle_base;

	public:
		// Underlying handle type. This is macro'd in Windows to be void* == (HANDLE)
		using object_type = Object_t;

		// Note: Process-local handle table starts at entry 4, hence the null ( == 0) 
		// entry is not a valid one. WINAPI functions tend to return NULL, though some
		// of them return INVALID_HANDLE_VALUE.
	public:
		// ***Explicit only at declaration, not definition
		// Only allow native coversion via explicit cast/ctor 
		constexpr explicit handle(native_type h, flag_type flags);
		constexpr explicit handle(native_type h);

		// invalid_handle literal ctor
		constexpr handle();
		constexpr handle(detail::invalid_t);

		// Only allow native coversion via explicit cast/ctor 

		// Move constructor
		template <typename other_t>
		handle(handle<other_t>&& other);
		
		// Move assignment
		template <typename other_t>
		handle& operator =(handle<other_t>&& other);
		
		// Covariant handle cast
		//template <typename other_t>
		//explicit operator const handle<other_t>&() const;

		// Close handle to windows object.
		// Handle must be weakly valid in order to close the handle.
		~handle() = default;

	private:
				// Allow attorney to expose some implementation details
		// This is mainly for recreated winapi functions to pass
		// the underlying handle value into the winapi.
		template <typename>
		friend class distant::detail::attorney::to_handle;

		// Provide any templated version of windows::handle access to implementation details
		// This is clearly the normal for objects under the same instantiation
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