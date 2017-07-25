#pragma once

#include <Tlhelp32.h>

#include <distant\windows\handle.hpp>

namespace distant {
namespace windows {
namespace kernel  {

	// Make this a CRTP (Or just tagged?) where any derived class is at least a windows::kernel::object
	// The snapshot ctor should tag dispatch based on the templated type
	// kernel::process should dispatch to CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	// Similary, other objects supported by system::snapshot should dispatch to their respective flag.
	class snapshot
	{
		//snapshot
	};

} // end namespace kernel
} // end namespace windows
} // end namespace distant