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

	class handle : public windows::gle
	{
	public:
		// Special type literals
		class invalid_t : public detail::Literal {};
		class null_t	: public detail::Literal {};

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
			m_flags(flags::close_protected)
		{}

		constexpr handle(null_t) :
			m_handle_value(under_null_handle),
			m_flags(flags::close_protected)
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
		// Numeric validity of the handle is required for closing the handle.
		~handle() { this->close_handle(); }

	public:
		// This weak validity should only be used for validating the handle's numeric value.
		// This does not ensure the handle is from a valid object.
		bool weakly_valid() const 
		{ return m_handle_value != under_null_handle; }

		bool closed() const { return m_closed; }

		// Close the handle, if it is numerically valid and its closing wasn't observed
		void close_handle()
		{
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
#if VER_PRODUCTBUILD > 9600 
			CompareObjectHandles(lhs.m_handle_value, rhs.m_handle_value) &&
#else
			lhs.m_handle_value == rhs.m_handle_value;
#endif
	}

	inline bool operator !=(const handle& lhs, const handle& rhs)
	{ 
		return !operator==(lhs, rhs); 
	}

	static constexpr handle::null_t    null_handle;
	static constexpr handle::invalid_t invalid_handle;

} // end namespace windows

using windows::handle;

} // end namespace distant