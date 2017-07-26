#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <distant\detail\fwd.hpp>
#include <distant\windows\handle.hpp>

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
		static HANDLE native_handle(const windows::handle& h)
		{ return h.native_handle(); }

		static void invalidate(windows::handle& h)
		{ h.invalidate(); }

		friend Accessor;
	};
	
} // end namespace attorney
} // end namespace detail
} // end namespace distant