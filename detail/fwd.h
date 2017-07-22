#pragma once

// Forward declarations of some public (non-implemention) classes.

#include <distant\windows\access_rights.h>

namespace distant {

template <windows::access_rights access_t = windows::access_rights::all_access>
class process;

namespace windows {
	 
class handle;
class wait;

namespace kernel  {

class object;
class securable;

} // end namespace 
} // end namespace 
} // end namespace 