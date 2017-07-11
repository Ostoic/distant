#pragma once

#include <distant\windows\handle.h>

namespace distant {
namespace windows {

	class object
	{
	public:
		using error_type  = DWORD;
		using handle_type = distant::handle_type;

	public:
		/*************************************/
		/** Windows object status functions **/
		/*************************************/
		// Implicitly cast object to handle_type
		operator const handle_type&()	const { return m_handle; }
		const handle_type& get_handle()	const { return m_handle; }

		// Get the last error code that was recorded
		error_type get_last_error() const { return m_error; }

		// Determine if the object handle is valid
		bool valid_handle() const
		{ return m_handle != invalid_handle; }

		/*********************************/
		/** Windows Object constructors **/
		/*********************************/
		// Invalid handle default constructor
		object() : m_handle(invalid_handle) {}

		// Pretty much a copy constructor
		object(const handle_type& handle) : m_handle(handle) {}

		// Close the windows handle on destruction
		~object() { this->close_handle(); }

	protected:
		// Invalidate our handle
		// This is used for swapping objects
		void invalidate()
		{
			m_handle = invalid_handle;
		}

		// Close process handle and invalidate process object
		// Mutates: from invalidate() 
		void close_handle()
		{
			if (this->valid_handle())
				CloseHandle(m_handle);

			this->invalidate();
		}

	protected:
		handle_type m_handle;
		error_type m_error;
	};

} // end namespace windows
} // end namespace distant