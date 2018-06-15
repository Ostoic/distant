// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/kernel_objects/process.hpp>
#include <distant/memory/virtual_ptr.hpp>
#include <distant/memory/virtual_reference.hpp>
#include <distant/access_rights.hpp>

namespace distant::memory
{
	template <typename T, typename AddressT = dword, process_rights AccessRights = process_rights::all_access>
	class virtual_allocator
	{
	public:
		using value_type		 = T;
		using size_type			 = typename std::allocator_traits<virtual_allocator>::size_type;
		using pointer			 = typename std::allocator_traits<virtual_allocator>::pointer;
		using const_void_pointer = typename std::allocator_traits<virtual_allocator>::const_void_pointer;

		//static_assert(AccessRights >= process_rights::vm_operation, "");

	public:
		virtual_allocator() noexcept : process_(nullptr) {}
		virtual_allocator(const virtual_allocator& other) noexcept : process_(other.process_) {}
		virtual_allocator(virtual_allocator&& other) noexcept: process_(other.process_) {}

		template <typename U, typename OtherAddressT, process_rights OtherRights>
		virtual_allocator(virtual_allocator<U, OtherAddressT, OtherRights>&& other) noexcept : process_(other.process_) {}

		template <typename U, typename OtherAddressT, process_rights OtherRights>
		virtual_allocator(const virtual_allocator<U, OtherAddressT, OtherRights>& other) noexcept : process_(other.process_) {}

		explicit virtual_allocator(process<AccessRights>& process) noexcept : process_(&process) {}

		virtual_allocator& operator=(const virtual_allocator& other) noexcept
		{
			process_ = other.process_;
			return *this;
		}

		virtual_allocator& operator=(virtual_allocator&& other) noexcept
		{
			*this = static_cast<virtual_allocator>(other);
			return *this;
		}

		void swap(virtual_allocator& other) noexcept
		{
			using std::swap;
			swap(process_, other.process_);
		}

		pointer allocate(const size_type count)
		{
			return distant::virtual_malloc<T>(*process_, count * sizeof(T));
		}

		pointer allocate(const size_type count, const_void_pointer hint)
		{
			return allocate(count);
		}

		void deallocate(pointer p, const size_type count) noexcept
		{
			virtual_free<T>(*process_, p);
		}

	private:
		template <typename Allocator>
		friend struct std::allocator_traits;

		process<AccessRights>* process_;
	};

	template <typename T, typename A, process_rights R>
	void swap(virtual_allocator<T, A, R>& lhs, virtual_allocator<T, A, R>& rhs) noexcept
	{
		lhs.swap(rhs);
	}

	template <typename T, typename A, process_rights R>
	bool operator==(virtual_allocator<T, A, R> lhs, virtual_allocator<T, A, R> rhs) noexcept
	{
		return *rhs.process_ == *lhs.process_;
	}

	template <typename T, typename A, process_rights R>
	bool operator!=(virtual_allocator<T, A, R> lhs, virtual_allocator<T, A, R> rhs) noexcept
	{
		return !operator==(lhs, rhs);
	}

} // namespace distant

namespace distant { using memory::virtual_allocator; }

namespace std
{
	template <typename T, typename AddressT, distant::process_rights AccessRights>
	struct allocator_traits<distant::memory::virtual_allocator<T, AddressT, AccessRights>>
	{
	private:
		template <typename U>
		using vptr_t = distant::virtual_ptr<U, AddressT, AccessRights>;

		template <typename U>
		using vref_t = distant::virtual_reference<U, AddressT, AccessRights>;

		template <typename U>
		using valloc_t = distant::memory::virtual_allocator<U, AddressT, AccessRights>;

		using valloc = valloc_t<T>;

	public:
		using value_type		 = T;
		using pointer			 = vptr_t<T>;
		using const_pointer		 = vptr_t<const T>;
		using void_pointer		 = vptr_t<void>;
		using const_void_pointer = vptr_t<const void>;
		using reference			 = vref_t<T>;
		using const_reference	 = vref_t<const T>;
		using size_type			 = std::size_t;
		using difference_type	 = std::ptrdiff_t;

		// Propagate allocator so that the underlying process is passed to the copied/swapped/moved container.
		using propagate_on_container_copy_assignment = true_type;
		using propagate_on_container_move_assignment = true_type;
		using propagate_on_container_swap			 = true_type;
		using is_always_equal						 = false_type;

		template<class Other>
		using rebind_alloc = valloc_t<Other>;

		template<class Other>
		using rebind_traits = allocator_traits<valloc_t<Other>>;

		template<typename U>
		struct rebind
		{
			using other = valloc_t<U>;
		};

		template <typename... Args>
		static void construct(valloc& allocator, pointer p, Args&&... args)
		{
			T value{ std::forward<Args>(args)... };

			distant::memory::write<T>(
				*allocator.process_, 
				p.get(), 
				std::move(value)
			);
		}

		static void destroy(valloc& allocator, pointer p)
		{
			// Todo: Figure out how to call the destructor of a class in another process?
			// Todo: memory::write has customization points that may change the representation of the type in the target process.
			//this->deallocate(p);
		}

		static valloc select_on_container_copy_construction(const valloc& allocator)
		{	
			return allocator;
		}

		static size_type max_size(const valloc& allocator)
		{
			return std::allocator_traits<std::allocator<T>>::max_size(std::allocator<T>{});
		}
	};
}
