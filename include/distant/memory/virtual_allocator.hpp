#pragma once

#include <distant/memory/virtual_ptr.hpp>
#include <distant/memory/virtual_reference.hpp>

namespace distant
{
	namespace memory
	{
		template <typename T>
		class virtual_allocator
		{
		
		};

	} // namespace memory

	using memory::virtual_allocator;

} // namespace distant


namespace std
{
	template <typename T>
	struct allocator_traits<distant::memory::virtual_allocator<T>>
	{
		
	};
}