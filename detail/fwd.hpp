#pragma once

// Forward declarations of some public (non-implemention) classes.

#include <distant\windows\access_rights.hpp>

namespace distant {

namespace windows {
	 
class handle;
class wait;

namespace kernel  {

class object;
class securable;

template <access_rights::process access_t = windows::access_rights::all_access>
class process;

} // end namespace 
} // end namespace 
} // end namespace 