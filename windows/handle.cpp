#include <distant\windows\handle.h>

namespace distant {
namespace windows {

	constexpr handle::handle(invalid_t) :
		m_handle(under_null_handle),
		m_flags(flags::close_protected)
	{}

	constexpr handle::handle(null_t) :
		m_handle(under_null_handle),
		m_flags(flags::close_protected)
	{}

	constexpr handle::handle() : handle(null_t()) {}

	handle::handle(value_type h) :
		m_handle(h),			
		m_flags(flags::inherit) // This allows the handle to be closed properly
	{}

	handle::handle(value_type h, flag_type flags) :
		m_handle(h), 
		m_flags(flags) 
	{}

	// Close handle to windows object.
	// Numeric validity of the handle is required for closing the handle.
	~handle::handle() { this->close(); }

	void handle::invalidate() { m_handle = under_null_handle; }

	// Close the handle, if it is numerically valid and its closing wasn't observed
	void handle::close()
	{
		if (!this->closed() && this->weakly_valid())
		{
			CloseHandle(m_handle);
			this->invalidate();
			this->update_gle();
		}

		m_closed = true;
	}

	handle::value_type handle::get_value() const { return m_handle; }

} // end namespace windows
} // end namespace distant