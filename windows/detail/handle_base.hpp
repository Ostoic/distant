#pragma once

#include <Windows.h>

#include <distant\windows\access_rights.hpp>

#include <distant\detail\attorney.hpp>
#include <distant\utility\literal.hpp>

namespace distant::windows {
namespace detail		   {

	// Forward declare invalid handle literal
	class invalid_t;

	// Implements the interface of windows::handle
	class handle_base
	{
	public:
		// Underlying handle type. This is macro'd in Windows to be void* == (HANDLE)
		using native_type = HANDLE;
		using flag_type = windows::access_rights::handle;

		// Note: Process-local handle table starts at entry 4, hence the null ( == 0) 
		// entry is not a valid one. WINAPI functions tend to return NULL, though some
		// of them return INVALID_HANDLE_VALUE.
	public:
		// ***Explicit only at declaration, not definition
		// Only allow native coversion via explicit cast/ctor 
		constexpr explicit handle_base(native_type h, flag_type flags);

		// Move copyable
		handle_base(handle_base&&);

		// Move assignable
		handle_base& operator=(handle_base&&);

		// If we allow copy ctor/assignment, then multiple copies
		// will eventually attempt to close the same handle, which
		// is not desirable.
		constexpr handle_base(const handle_base&) = delete;
		handle_base& operator =(const handle_base&) = delete;

		// Close handle to windows object.
		// Handle must be weakly valid in order to close the handle.
		~handle_base() { this->close(); }

	public:
		// This weak validity should only be used for validating the handle's numeric value.
		// This does not ensure the handle is from a valid object.
		bool valid() const;

		// Disable the ability to close the handle
		bool close_protected() const;

		// Check if handle's closure has been observed
		bool closed() const;

		// Close the handle, if it is valid and its closure wasn't observed
		// Note: This function is public since handles occasionally need to be closed before the
		// stack unwind (I think).
		// XXX Consider protecting close_handle
		void close();

	protected:
		// Numerically invalidate and close protect our handle.
		// According to "Windows Via C\C++" by Jeffrey Richter,
		// setting the handle to null is preferable to invalid_handle
		// after closing the handle. This is probably because some API
		// calls take invalid_handle as the current process.
		void invalidate();

		// Protect the handle from being closed
		void protect();

		// Allow derived classes to interface with the handle value itself.
		// This allows us to make API calls at a higher inheritance level.
		native_type native_handle() const { return m_native_handle; }
		flag_type flags()  const { return m_flags; }

	protected:
		// HANDLE value
		native_type m_native_handle;

		// Handle close protection flags
		flag_type m_flags;

		// If we somehow attempt to call CloseHandle multiple times,
		// this will help prevent further unnecessary calls.
		bool m_closed = false;

	public:
		friend constexpr bool operator ==(const handle_base&, const handle_base&);
		friend constexpr bool operator !=(const handle_base&, const handle_base&);
	};

	class invalid_t : public utility::Literal<invalid_t> {};

	// Type-safe handle literal
	constexpr typename detail::invalid_t invalid_handle;

} // end namespace detail

	using detail::invalid_handle;

} // end namespace distant::windows

#include <distant\windows\detail\handle_base.inl>
