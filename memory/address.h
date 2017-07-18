#pragma once

#include <cstdint>

#include <distant\detail\literal.h>

namespace distant {
namespace memory  { 

	// Address type that is large enough to hold 64-bit addresses, while still
	// conforming with WINAPI usage.
	
	class address
	{
	public:
		using value_type = std::uintptr_t;
		
		class null_t : public detail::Literal {};
	
	public:
		address() : address(null_t()) {}
		address(null_t) : m_address(0) {}
		address(value_type adr) : m_address(adr) {}
	
	private:
		value_type m_address;
	};

	static constexpr address::null_t null_address;
	//address_type null_address(0);
	
} // end namespace memory
} // end namespace distant