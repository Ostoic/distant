#pragma once

#include <Windows.h>

// For Windows SDK selection
#include <ntverp.h>

#include <distant\windows\gle.h>
#include <distant\detail\literal.h>
#include <distant\windows\kernel\object.h>

#include <functional>

namespace distant {

using handle_type = HANDLE;

namespace windows {

	namespace kernel { class object; }

	static constexpr handle_type under_null_handle = NULL;

	// windows::handle is a type-safe version of the WINAPI defined macro: HANDLE
	class handle : public windows::gle
	{
	public:
		// Type-safe handle literals
		class invalid_t : public detail::Literal {};
		class null_t	: public detail::Literal {}; 

		
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
		using value_type = distant::handle_type;
		using flag_type  = handle::flags;

	public:
		constexpr handle(invalid_t) :
			m_handle_value(under_null_handle),
			m_flags(flags::close_protected) // Close on invalid handle is invalid
		{}

		constexpr handle(null_t) :
			m_handle_value(under_null_handle),
			m_flags(flags::close_protected) // Close on null handle is invalid
		{}

		constexpr handle() : handle(null_t()) {}

		handle(value_type h) :
			m_handle_value(h),
			m_flags(flags::inherit) // This allows the handle to be closed properly
		{}

		handle(value_type h, flag_type flags) :
			m_handle_value(h),
			m_flags(flags) 
		{}

		// Close handle to windows object.
		// Handle must be weakly valid in order to close the handle.
		~handle() { this->close_handle(); }

	public:
		// This weak validity should only be used for validating the handle's numeric value.
		// This does not ensure the handle is from a valid object.
		bool weakly_valid() const 
		{ return m_handle_value != under_null_handle; }

		bool closed() const { return m_closed; }

		// Close the handle, if it is weakly valid and its closure wasn't observed
		// Note: This function is public since 
		void close_handle()
		{
			// TODO: Query WinAPI for kernel object reference count
			// If this reference count > 0, then continue.
			// But if the reference count == 0, the system should destroy
			// the object regardless?
			if (!this->closed() && this->weakly_valid())
			{
				CloseHandle(m_handle_value);
				this->invalidate();
				this->update_gle();
			}

			m_closed = true;
		}

	protected:
		// Numerically invalidate the handle.
		// According to "Windows Via C\C++" by Jeffrey Richter,
		// setting the handle to null is preferable to invalid_handle
		// after closing the handle.
		void invalidate() { m_handle_value = under_null_handle; }

		// Allow derived classes to interface with the handle value itself.
		// This allows us to make API calls at a higher inheritance level.
		value_type get_value() const { return m_handle_value; }

	private:
		value_type m_handle_value;
		flag_type m_flags;

		bool m_closed = false;

		// Allow object to view implementation details;
		friend class wait;
		friend class kernel::object;

	public:
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
			lhs.m_handle_value == rhs.m_handle_value;
#endif
	}

	inline bool operator !=(const handle& lhs, const handle& rhs)
	{ return !operator==(lhs, rhs); }

	// Type-safe handle literals
	static constexpr handle::null_t    null_handle;
	static constexpr handle::invalid_t invalid_handle;

} // end namespace windows

using windows::handle;

} // end namespace distant