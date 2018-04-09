// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <distant/fwd.hpp>

#include <boost/winapi/basic_types.hpp>

namespace distant::detail::attorney  {

	// Attorney class that allows the Client to view an implementation detail of the given class.
	template <class Accessor>
	class to_handle
	{
	public:
		constexpr to_handle() = default;

	private:
		template <class object_t>
		static boost::winapi::HANDLE_ native_handle(const handle<object_t>& h)
		{ return h.native_handle(); }

		template <class object_t>
		static void invalidate(handle<object_t>& h)
		{ h.invalidate(); }

		friend Accessor;
	};
	
} // end namespace distant::detail::attorney