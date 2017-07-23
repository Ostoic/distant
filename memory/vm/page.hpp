#pragma once

#include <distant\windows\gle.h>
#include <distant\memory\address.h>

namespace distant {
namespace memory  {

	// Perhaps use policy-based system to inherit windows 
	// get-page-size function
	class page : public windows::gle
	{
	public:
		using address_type = memory::address_type;
		using size_type    = std::uintptr_t;

		page() : m_start(null_address), m_size(0) {}
		page(address_type start, size_type size) : m_start(start), m_size(size) {}

	protected:
		address_type m_start;
		size_type m_size;
	};

} // end namespace memory
} // end namespace distant