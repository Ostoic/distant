#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <Windows.h>

// For Windows SDK selection
#include <ntverp.h>

#include <distant\windows\error\gle.hpp>
#include <distant\utility\literal.hpp>
#include <distant\utility\type_traits.hpp>

#include <distant\detail\attorney.hpp>

#include <distant\windows\access_rights.hpp>

namespace distant {
namespace windows {

	namespace detail {
		class invalid_t;
	}

	// windows::handle is a type-safe version of the WINAPI defined macro: HANDLE
	template <typename Object_t> // Encode the object type into the handle for type safety
	class handle : public error::gle
	{
	public:
		// Underlying handle type. This is macro'd in Windows to be void* == (HANDLE)
		using object_type = Object_t;
		using native_type = HANDLE;
		using flag_type   = access_rights::handle;

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
		constexpr handle(const detail::invalid_t&);

		// Only allow native coversion via explicit cast/ctor 

		/*typename enable_if<!is_array<_Ty2>::value
			&& is_assignable<_Dx&, _Dx2&&>::value
			&& is_convertible<typename unique_ptr<_Ty2, _Dx2>::pointer,
			pointer>::value,
			_Myt&>::type*/

		// Move constructor
		template <typename other_t>
		handle(handle<other_t>&& other);
		
		// Move assignment
		template <typename other_t>
		handle& operator =(handle<other_t>&& other);
		
		// Covariant handle cast
		template <typename other_t>
		explicit operator const handle<other_t>&() const;

		// If we allow copy ctor/assignment, then multiple copies
		// will eventually attempt to close the same handle, which
		// is not desirable.
		constexpr handle(const handle&) = delete;
		handle& operator =(const handle&) = delete;

		// Close handle to windows object.
		// Handle must be weakly valid in order to close the handle.
		~handle() { this->close_handle(); }

	public:
		// This weak validity should only be used for validating the handle's numeric value.
		// This does not ensure the handle is from a valid object.
		bool close_protected() const;
		bool closed() const;
		bool weakly_valid() const;

		// Close the handle, if it is weakly valid and its closure wasn't observed
		// Note: This function is public since handles occasionally need to be closed before the
		// stack unwind.
		// XXX Consider protecting close_handle
		void close_handle();

	protected:
		// Numerically invalidate and close protect our handle.
		// According to "Windows Via C\C++" by Jeffrey Richter,
		// setting the handle to null is preferable to invalid_handle
		// after closing the handle. This is probably because some API
		// calls take invalid_handle as the current process.
		void invalidate();
		void close_protect();

		// Allow derived classes to interface with the handle value itself.
		// This allows us to make API calls at a higher inheritance level.
		native_type native_handle() const { return m_native_handle; }
		flag_type flags()  const { return m_flags; }

	private:
		// HANDLE value
		native_type m_native_handle;

		// Handle close protection flags
		flag_type m_flags;

		// If we somehow attempt to call CloseHandle multiple times,
		// this will help prevent further unnecessary calls.
		bool m_closed = false;

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

	namespace detail {
		class invalid_t : public utility::Literal<invalid_t> {};
	}

	// Type-safe handle literal
	constexpr typename detail::invalid_t invalid_handle;

} // end namespace windows

using windows::handle;
using windows::invalid_handle;

} // end namespace distant

#include <distant\windows\detail\handle.inl>