#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <distant/config.hpp>
#include <distant/types.hpp>
#include <distant/handle.hpp>
#include <distant/kernel/process.hpp>
#include <distant/kernel/access_token.hpp>
#include <distant/system/snapshot.hpp>
#include <distant/system/information.hpp>

#include <distant/memory/address.hpp>
#include <distant/detail/to_string.hpp>

// Plans:
//		- Implement "virtual" memory allocator that models the standard Allocator concept properly.
//		  Consider Alexandrescu's composable allocator design.
//		- Finish system::snapshot for threads, heaps, modules
//		- Fix up memory::vm to work with the new versions of distant::process and distant::handle
