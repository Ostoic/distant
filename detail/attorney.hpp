#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <distant\detail\fwd.hpp>
//#include <distant\windows\handle.hpp>

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
		template <class object_t>
		static HANDLE native_handle(const windows::handle<object_t>& h)
		{ return h.native_handle(); }

		template <class object_t>
		static void invalidate(windows::handle<object_t>& h)
		{ h.invalidate(); }

		friend Accessor;
	};
	
} // end namespace attorney
} // end namespace detail
} // end namespace distant