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

#include <distant\access_rights.hpp>

namespace distant {
namespace error   {

class windows_error_code;

} // namespace error
	 
template <typename>
class handle;

class wait;

namespace kernel {

class object;
class securable;

// Default template argument is all_access
template <access_rights::process access_t = access_rights::process::all_access>
class process;

} // namespace kernel

namespace system {

	template <typename, typename>
	class snapshot;

} // namespace system

} // namespace distant