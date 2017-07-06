#pragma once

#include <distant\windows\handle.h>

namespace distant {
namespace windows {

	// Base class for any windows object
	// Example objects are the following:
	/************************************//*
		Change notification
		Console input
		Event
		Memory resource notification
		Mutex
		Process
		Semaphore
		Thread
		Waitable timer
	/************************************/
	// Go to MSDN for more information
	class synchro
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
		synchro() : m_handle(invalid_handle), m_error() {}

		// Pretty much a copy constructor
		synchro(const handle_type& handle) : m_handle(handle), m_error() {}

		// Close the windows handle on destruction
		~synchro() { this->close_handle(); }

		friend bool operator ==(const synchro&, const synchro&);
		friend bool operator !=(const synchro&, const synchro&);

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
			{
				CloseHandle(m_handle);
				this->invalidate();
			}
		}

	protected:
		handle_type m_handle;
		error_type m_error;
	};
	
	inline bool operator ==(const synchro& lhs, const synchro& rhs)
	{
		return lhs.m_handle == rhs.m_handle;
	}

	inline bool operator !=(const synchro& lhs, const synchro& rhs)
	{
		return !operator==(lhs, rhs);
	}


} // end namespace windows
} // end namespace distant