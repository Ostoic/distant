#pragma once

#include <distant/kernel/process.hpp>
#include <distant/memory/virtual_ptr.hpp>
#include <distant/memory/virtual_reference.hpp>

namespace distant
{
	namespace memory
	{
		template <typename T, process_rights Access, typename AddressT>
		class virtual_allocator
		{
		public:
			using value_type	  = T;
			using pointer		  = virtual_ptr<T, AddressT>;
			using const_pointer	  = virtual_ptr<const T, AddressT>;
			using reference		  = virtual_reference<T, AddressT>;
			using const_reference = virtual_reference<const T, AddressT>;
			using size_type		  = std::size_t;
			using difference_type = address<AddressT>;

			template<typename U>
			struct rebind
			{
				using other = virtual_allocator<U, Access, AddressT>;
			};

		public:
			virtual_allocator() = default;
			virtual_allocator(const process<Access>& process) noexcept;

			pointer allocate(size_type count);

			void deallocate(pointer p, size_type count);

			size_type max_size() const noexcept;

		private:
			const process<Access>* process_;
		};


	} // namespace memory

	using memory::virtual_allocator;

} // namespace distant


namespace std
{
	/*template <typename T>
	struct allocator_traits<distant::memory::virtual_allocator<T>>
	{
		
	};*/
}

#include "impl/virtual_allocator.hxx"