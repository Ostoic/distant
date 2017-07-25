#pragma once

/*!
@file
Includes all the library components except the adapters for external
libraries.

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

	// windows::handle is a type-safe version of the WINAPI defined macro: HANDLE
	class handle : private error::gle
	{
	public:
		// Type-safe handle literals
		class invalid_t : public utility::Literal {};
		class null_t	: public utility::Literal {};

		// Note: Process-local handle table starts at entry 4, hence the null ( == 0) 
		// entry is not a valid one. WINAPI functions tend to return NULL, though some
		// of them return INVALID_HANDLE_VALUE.
	public:
		// Windows Handle flags
		enum class flags
		{
			inherit = HANDLE_FLAG_INHERIT,					  // Child process will inherit object handle
			close_protected = HANDLE_FLAG_PROTECT_FROM_CLOSE, // Prevent CloseHandle from closing handle
		};

	public:
		// Underlying handle type. This is macro'd in Windows to be void* == (HANDLE)
		using native_handle_type = HANDLE;
		using flag_type  = handle::flags;

	public:
		constexpr handle(invalid_t) :
			m_native_handle(NULL),
			m_flags(flags::close_protected), // Closing invalid handle is not allowed
			m_closed(false)
		{}

		constexpr handle(null_t) :
			m_native_handle(NULL),
			m_flags(flags::close_protected), // Closing null handle is not allowed
			m_closed(false)
		{}

		constexpr handle() :
			m_native_handle(NULL),
			m_flags(flags::close_protected), // Closing null handle is not allowed
			m_closed(false)
		{}

		// Only allow conversion to underlying type through an explicit cast/ctor 
		constexpr explicit handle(native_handle_type h) :
			m_native_handle(h),
			m_flags(flags::inherit), // This allows the handle to be closed properly
			m_closed(false)
		{}

		// Only allow conversion to underlying type through an explicit cast/ctor 
		constexpr explicit handle(native_handle_type h, flag_type flags) :
			m_native_handle(h),
			m_flags(flags),
			m_closed(false)
		{}

		handle(handle&& other) : 
			m_native_handle(std::move(other.m_native_handle)),
			m_flags(std::move(other.m_flags)),
			m_closed(std::move(other.m_closed))
		{ other.invalidate(); }
		
		handle& operator =(handle&& other)
		{
			m_closed = other.m_closed;
			m_flags = other.m_flags;
			m_native_handle = other.m_native_handle;
			other.invalidate();
			// other should die very soon since it is an rvalue
			return *this;
		}

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
		bool weakly_valid() const 
		{ return m_native_handle != NULL; }

		bool close_protected() const
		{ return m_flags == flags::close_protected;}

		bool closed() const { return m_closed; }

		// Close the handle, if it is weakly valid and its closure wasn't observed
		// Note: This function is public since handles occasionally need to be closed before the
		// stack unwind.
		void close_handle()
		{
			// TODO: Query WinAPI for kernel object reference count
			// If this reference count > 0, then continue.
			// But if the reference count == 0, the system should destroy
			// the object regardless?
			if (!this->close_protected() &&
				!this->closed()			 &&
				 this->weakly_valid())
			{
				CloseHandle(m_native_handle);
				this->invalidate();
				this->update_gle();
			}

			m_closed = true;
		}

	protected:
		// Numerically invalidate and close protect our handle.
		// According to "Windows Via C\C++" by Jeffrey Richter,
		// setting the handle to null is preferable to invalid_handle
		// after closing the handle. This is probably because some API
		// calls take invalid_handle as the current process.
		void invalidate() 
		{ 
			close_protect();
			m_native_handle = NULL;
		}
		
		void close_protect()
		{
			m_flags = flags::close_protected;
		}

		// Allow derived classes to interface with the handle value itself.
		// This allows us to make API calls at a higher inheritance level.
		native_handle_type native_handle() const { return m_native_handle; }
		flag_type flags()  const { return m_flags; }

	private:
		native_handle_type m_native_handle;
		flag_type m_flags;

		bool m_closed = false;

		// Allow attorney to expose some implementation details
		// This is mainly for recreated winapi functions to pass
		// the underlying handle value into the winapi.
		template <typename>
		friend class detail::attorney::to_handle;

	public:
		friend void swap(handle& lhs, handle& rhs)
		{
			using std::swap;
			swap(lhs.m_native_handle, rhs.m_native_handle);
			swap(lhs.m_closed, rhs.m_closed);
			swap(lhs.m_flags, rhs.m_flags);
		}

		friend bool operator ==(const handle&, const handle&);
		friend bool operator !=(const handle&, const handle&);
	};

	inline bool operator ==(const handle& lhs, const handle& rhs)
	{
		return
		
		// CompareObjectHandles is only available with the Windows 10
		// SDK or higher. 
#if VER_PRODUCTBUILD > 9600 
			CompareObjectHandles(lhs.m_handle_value, rhs.m_handle_value) &&
#else	// Otherwise, we just compare the handle values.
			lhs.m_native_handle == rhs.m_native_handle;
#endif
	}

	inline bool operator !=(const handle& lhs, const handle& rhs)
	{ return !operator==(lhs, rhs); }

	// Type-safe handle literals
	static constexpr handle::null_t    null_handle;
	static constexpr handle::invalid_t invalid_handle;

} // end namespace windows

using windows::handle;
using windows::null_handle;
using windows::invalid_handle;

} // end namespace distant