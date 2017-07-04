#pragma once

#include <type_traits>

#include <Windows.h>

namespace distant {
namespace memory  { 

	class address
	{
	public:
		using type = DWORD;

	public:
		address(type addr) : m_address(addr) {}

		operator type () const { return m_address; }

	private:
		type m_address;
	};

} // end namespace memory
} // end namespace distant