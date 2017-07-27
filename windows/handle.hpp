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
#include <distant\detail\attorney.hpp>

#include <iostream>

namespace distant {
namespace windows {

	namespace detail {

		// Type-safe handle literals
		class invalid_t : public utility::Literal {};
		class null_t : public utility::Literal {};
	}

	// windows::handle is a type-safe version of the WINAPI defined macro: HANDLE
	template <typename Object_t> // Encode the object type into the handle for type safety
	class handle : private error::gle
	{
	public:
		// Windows Handle flags
		enum class flags
		{
			inherit = HANDLE_FLAG_INHERIT,					  // Child process will inherit object handle
			close_protected = HANDLE_FLAG_PROTECT_FROM_CLOSE, // Prevent CloseHandle from closing handle
		};

	public:
		// Underlying handle type. This is macro'd in Windows to be void* == (HANDLE)
		using object_type = Object_t;
		using native_type = HANDLE;
		using flag_type   = flags;

		// Note: Process-local handle table starts at entry 4, hence the null ( == 0) 
		// entry is not a valid one. WINAPI functions tend to return NULL, though some
		// of them return INVALID_HANDLE_VALUE.
	public:
		constexpr handle(detail::invalid_t);
		
		constexpr handle(detail::null_t);
		
		constexpr handle();

		// Only allow conversion to underlying type through an explicit cast/ctor 
		constexpr explicit handle(native_type h);

		// ***Explicit only at declaration, not definition
		// Only allow conversion to underlying type through an explicit cast/ctor 
		constexpr explicit handle(native_type h, flag_type flags);

		// Move constructor
		template <typename other_object_t>
		handle(handle<other_object_t>&& other);
		
		// Move assignment
		template <typename other_object_t>
		handle& operator =(handle<other_object_t>&& other);

		// If we allow copy ctor/assignment, then multiple copies
		// will eventually attempt to close the same handle, which
		// is not desirable.
		constexpr handle(const handle&) = delete;
		handle& operator =(const handle&) = delete;

		// Close handle to windows object.
		// Handle must be weakly valid in order to close the handle.
		~handle() { this->close_handle(); }

	public:
		using gle::update_gle;

		// This weak validity should only be used for validating the handle's numeric value.
		// This does not ensure the handle is from a valid object.
		bool weakly_valid() const;
		bool close_protected() const;
		bool closed() const;

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

	public:
		/*friend void swap(handle& lhs, handle& rhs)
		{
			using std::swap;
			swap(lhs.m_native_handle, rhs.m_native_handle);
			swap(lhs.m_closed, rhs.m_closed);
			swap(lhs.m_flags, rhs.m_flags);
		}*/

		template <typename T, typename U>
		friend bool operator ==(const handle<T>&, const handle<U>&);

		template <typename T, typename U>
		friend bool operator !=(const handle<T>&, const handle<U>&);
	};

	template <typename T, typename U>
	inline bool operator ==(const handle<T>& lhs, const handle<U>& rhs)
	{
		// Objects must be compatible.
		// Example: thread ~/~ process, but process ~ securable
		static_assert(
			std::is_convertible_v<T, U>, // XXX Revise type check
			"Handle equality operator: Object types must be compatible");

		return
		// CompareObjectHandles is only available with the Windows 10
		// SDK or higher. 
#if VER_PRODUCTBUILD > 9600 
			CompareObjectHandles(lhs.m_handle_value, rhs.m_handle_value) &&
#else	// Otherwise, we just compare the handle values.
			lhs.m_native_handle == rhs.m_native_handle;
#endif
	}

	template <typename T, typename U>
	inline bool operator !=(const handle<T>& lhs, const handle<U>& rhs)
	{ return !operator==(lhs, rhs); }

	// Type-safe handle literals
	static constexpr typename detail::null_t	null_handle;
	static constexpr typename detail::invalid_t invalid_handle;

} // end namespace windows

using windows::handle;
using windows::null_handle;
using windows::invalid_handle;

} // end namespace distant

#include <distant\windows\detail\handle.inl>