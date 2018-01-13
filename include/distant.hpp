#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#ifndef BOOST_USE_WINDOWS_H
#define BOOST_USE_WINDOWS_H
#endif

#include <distant\config.hpp>
#include <distant\handle.hpp>
#include <distant\kernel\process.hpp>
#include <distant\system\snapshot.hpp>

namespace distant {
	using kernel::process;
	using system::snapshot;
}

// Plans:
//		- Implement "virtual" memory allocator that models the standard Allocator concept properly.
//		  Consider Alexandrescu's composable allocator design.
//		- Finish system::snapshot for threads, heaps, modules
//		- Fix up memory::vm to work with the new versions of distant::process and distant::handle
