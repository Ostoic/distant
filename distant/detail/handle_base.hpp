#pragma once

#include <Windows.h>

#include <distant\access_rights.hpp>

#include <distant\detail\attorney.hpp>
#include <distant\utility\literal.hpp>

namespace distant {
namespace detail  {

	// Forward declare invalid handle literal
	class invalid_t;

	// Implements the interface of handle
	class handle_base
	{
	public:
		// Underlying handle type. This is macro'd in Windows to be void* == (HANDLE)
		using native_type = HANDLE;
		using flag_type = access_rights::handle;

	public:
		/// Only allow native coversion via explicit cast/ctor 
		constexpr explicit handle_base(native_type h, flag_type flags = flag_type::inherit) noexcept;

		/// Move copyable
		handle_base(handle_base&&) noexcept;

		/// Move assignable
		handle_base& operator=(handle_base&&) noexcept;

		// If we allow copy ctor/assignment, then multiple copies
		// will eventually attempt to close the same handle, which
		// is not desirable.
		constexpr handle_base(const handle_base&) = delete;
		handle_base& operator =(const handle_base&) = delete;

		/// Close handle to windows object.
		/// Handle must be weakly valid in order to close the handle.
		~handle_base() { this->close(); }
		
	public:
		/// Checks the if the native handle is valid
		/// \return true if the native_handle is not NULL, and false otherwise
		bool valid() const noexcept;

		/// Explicit bool operator wrapper for valid()
		explicit operator bool() const noexcept;

		/// Check if the handle is close protected
		/// \return true if the handle cannot be closed, false otherwise
		bool close_protected() const noexcept;

		/// Check if handle's closure has been observed
		// Note: This function is public since handles occasionally need to be closed before the
		// stack unwind (I think).
		/// \return true if the handle's closure was observed, and false otherwise
		bool closed() const noexcept;

		/// Close the handle, if it is valid and its closure wasn't observed
		void close() noexcept;


	protected:

		// According to "Windows Via C\C++" by Jeffrey Richter,
		// setting the handle to null is preferable to invalid_handle
		// after closing the handle. This is probably because some API
		// calls take invalid_handle as the current process.
		/// Numerically invalidate and close protect our handle.
		void invalidate() noexcept;

		/// Protect the handle from being closed
		void protect() noexcept;

		// Allow derived classes to interface with the handle value itself.
		// This allows us to make API calls at a higher inheritance level.

		/// Get the value of the native handle
		/// \return value of the native handle
		native_type native_handle() const noexcept;

		/// Get the handle's flag type
		/// \return distant::access_rights::handle flag type
		flag_type flags()  const noexcept;

	protected:
		/// native HANDLE value
		native_type m_native_handle;

		/// Handle close protection flags
		flag_type m_flags;

		// If we somehow attempt to call CloseHandle multiple times,
		// this will help prevent further unnecessary calls.
		/// Switch to check if closure was observed 
		bool m_closed = false;

	public:
		friend constexpr bool operator ==(const handle_base&, const handle_base&) noexcept;
		friend constexpr bool operator !=(const handle_base&, const handle_base&) noexcept;
	};

	class invalid_t : public utility::Literal<invalid_t> {};

	/// Type-safe handle literal
	constexpr typename detail::invalid_t invalid_handle;

} // end namespace detail

using detail::invalid_handle;

} // end namespace distant

#include <distant\detail\handle_base.inl>

  // Remarks:
  //		Process-local handle table starts at entry 4, hence the null ( == 0) 
  // entry is not a valid one. WINAPI functions tend to return NULL, though some
  // of them return INVALID_HANDLE_VALUE.