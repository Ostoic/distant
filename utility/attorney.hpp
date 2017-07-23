#pragma once

/*!
@file
Includes all the library components except the adapters for external
libraries.

@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <distant\detail\fwd.hpp>
#include <distant\windows\handle.hpp>

namespace distant  {
namespace utility  {
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
} // end namespace utility
} // end namespace distant