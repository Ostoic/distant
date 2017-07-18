#pragma once

#include <distant\detail\fwd.h>
#include <distant\windows\handle.h>

namespace distant  {
namespace detail   {
namespace attorney {

	// Attorney class that allows the Client to view an implementation detail of the given class.
	template <class Accessor>
	class to_handle
	{
	public:
		constexpr to_handle() = default;

	private:
		using result_type = HANDLE;

		static result_type get_value(const windows::handle& h)
		{ return h.get_value(); }

		static void invalidate(windows::handle& h)
		{ h.invalidate(); }

		friend Accessor;
	};
	
} // end namespace attorney
} // end namespace detail
} // end namespace distant