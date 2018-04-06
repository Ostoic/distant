// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

namespace distant::memory
{
	template <typename T, process_rights A, typename Ad>
	virtual_allocator<T, A, Ad>::virtual_allocator(const process<A>& process) noexcept
		: process_(process&)
	{}

	template <typename T, process_rights A, typename Ad>
	typename virtual_allocator<T, A, Ad>::pointer 
	virtual_allocator<T, A, Ad>::allocate(size_type count)
	{
		
	}

	template <typename T, process_rights A, typename Ad>
	void virtual_allocator<T, A, Ad>::deallocate(pointer p, size_type count)
	{
		
	}

	template <typename T, process_rights A, typename Ad>
	typename virtual_allocator<T, A, Ad>::size_type 
	virtual_allocator<T, A, Ad>::max_size() const noexcept
	{
		
	}

}
