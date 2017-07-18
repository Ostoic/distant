#pragma once

#include <distant\windows\handle.h>
#include <distant\windows\gle.h>

//#include <distant\detail\fwd.h>

namespace distant {
namespace windows {
namespace kernel  {

	// Go to MSDN for more information
	// Base kernel object for windows
	class object : public windows::gle
	{
	public:
		using error_type  = gle::error_type;
		using handle_type = windows::handle;

	public:
		/*************************************/
		/** Windows object status functions **/
		/*************************************/
		// Implicitly cast object to handle_type
		const handle_type& get_handle()	const { return m_handle; }
		operator const handle_type&()	const { return get_handle(); }

		// Determine if the object handle is valid
		bool weakly_valid() const
		{ return m_handle.weakly_valid(); }

		/*********************************/
		/** Windows Object constructors **/
		/*********************************/
		// Invalid handle default constructor
		constexpr object() : gle(), m_handle(invalid_handle) {}

		explicit object(handle_type&& h) : 
			m_handle(std::move(h))
		{}

		object(object&& other) :
			object(std::move(other.m_handle))
		{}
 
		// Calls handle destructor
		~object() {}

		friend bool operator ==(const kernel::object&, const kernel::object&);
		friend bool operator !=(const kernel::object&, const kernel::object&);

		friend void swap(object& lhs, object& rhs)
		{
			using std::swap;

			swap(lhs.m_handle, rhs.m_handle);
			swap(lhs.m_error, rhs.m_error);
		}

	protected:
		void close_object()
		{ m_handle.close_handle(); }

		handle_type& get_handle() { return m_handle; }

	protected:
		handle_type m_handle;
	};
	
	inline bool operator ==(const kernel::object& lhs, const kernel::object& rhs)
	{
		return windows::operator==(lhs.m_handle, rhs.m_handle);
	}

	inline bool operator !=(const kernel::object& lhs, const kernel::object& rhs)
	{
		return windows::operator!=(lhs.m_handle, rhs.m_handle);
	}

} // end namespace kernel
} // end namespace windows
} // end namespace distant

