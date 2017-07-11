#pragma once

#include <distant\windows\handle.h>
#include <distant\windows\gle.h>

#include <memory>

namespace distant {
namespace windows {

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
	// Base synchronization object for windows
	class synchro : public windows::gle
	{
	public:
		using error_type  = gle::error_type;
		using handle_type = distant::handle_type;

	public:
		/*************************************/
		/** Windows object status functions **/
		/*************************************/
		// Implicitly cast object to handle_type
		const handle_type& get_handle()	const { return m_handle; }
		operator const handle_type&()	const { return get_handle(); }

		// Determine if the object handle is valid
		bool valid_handle() const
		{ return m_handle != invalid_handle; }

		/*********************************/
		/** Windows Object constructors **/
		/*********************************/
		// Invalid handle default constructor
		constexpr synchro() : gle(), m_handle(invalid_handle) {}

		synchro(const synchro& other) = delete;

		// Pretty much a copy constructor
		constexpr synchro(handle_type&& handle) : gle(), m_handle(std::move(handle)) {}

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
		//handle_type m_handle;

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