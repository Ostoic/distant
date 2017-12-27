#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <Windows.h>

#include <distant\handle.hpp>
#include <distant\kernel\process.hpp>
#include <distant\system\snapshot.hpp>

namespace distant {
	using kernel::process;
}

// Plans:
//		- Implement "virtual" memory allocator that models the standard Allocator concept properly.
//		  Also follows Alexandrescu's composable allocator design.
//		- Finish system::snapshot for threads, heaps, modules
//		- Fix up memory::vm to work with the new versions of distant::process and distant::handle

// Ideas
// XXX Consider implementing error::gle as a derivation of std::error_code / std::error_conditional.
// XXX My initial thoughts were to avoid virtual functions, since they probably won't be needed.
// XXX However, this isn't too much of a problem since error handling should not be called in a
// XXX computation-intensive manner (I think).

// XXX Consider caching process file_path / name for efficiency.
