#pragma once

#include <cstdint>

namespace distant {
namespace memory  { 

	// Address type that is large enough to hold 64-bit addresses, while still
	// conforming with WINAPI usage.
	using address_type = std::uintptr_t;

	address_type null_address(0);
	
} // end namespace memory
} // end namespace distant