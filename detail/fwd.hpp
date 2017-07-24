#pragma once

/*!
@file
Includes all the library components except the adapters for external
libraries.

@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

// Forward declarations of some public (non-implemention) classes.

#include <distant\windows\access_rights.hpp>

namespace distant {
namespace windows {
namespace error   {

class gle;

} // end namespace error
	 
class handle;
class wait;

namespace kernel {

	class object;
	class securable;

	// Default template argument is all_access
	template <access_rights::process access_t = access_rights::process::all_access>
	class process;

} // end namespace kernel

} // end namespace windows
} // end namespace distant