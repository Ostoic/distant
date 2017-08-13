#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <distant\windows\handle.hpp>
#include <distant\windows\kernel\process.hpp>
#include <distant\windows\system\snapshot.hpp>

namespace distant {
	using namespace windows;
	using kernel::process;
}

// Plans:
//		- Implement "virtual" memory allocator that models the standard Allocator concept properly.
//		  Also follows Alexandrescu's composable allocator design.
//		- Finish system::snapshot for threads, heaps
//		- Fix up memory::vm to work with the new versions of distant::process and distant::handle
//		- Find way to reduce code bloat in distant::process. This isn't currently a problem, but seeing 
//		  as each function is copied for each unique access_right combination, this could lead to considerable 
//		  bloat. A possible fix is to reduce the size of member functions of distant::process, and delegate most
//		  of the work out to "implementation-detail" free functions.
//		  Examples: http://imgur.com/a/97IAD

// Ideas
// XXX Consider implementing error::gle as a derivation of std::error_code / std::error_conditional.
// XXX My initial thoughts were to avoid virtual functions, since they probably won't be needed.
// XXX However, this isn't too much of a problem since error handling should not be called in a
// XXX computation-intensive manner (I think).

// XXX Consider caching process path name / name for efficiency.
