#pragma once

#include <distant/memory/virtual_ptr.hpp>
#include <distant/memory/virtual_memory.hpp>

namespace distant::memory
{
	template <class T, class AddressT, process_rights Access>
	class unique_ptr
	{
	public:
		using address_t = address<AddressT>;
		using pointer = virtual_ptr<T, AddressT, Access>;

	public:
		explicit operator bool() const noexcept { return this->get() != 0; }

		auto operator*() const noexcept { return *ptr_; }

		address_t get() const noexcept { return ptr_.get(); }

		address_t release() noexcept
		{
			auto result = this->get();
			ptr_ = nullptr;
			return result;
		}

		void reset(pointer p = {}) noexcept
		{
			pointer old = ptr_;
			ptr_ = std::move(p);
			if (old != pointer{})
				virtual_free(old);
		}

		void swap(unique_ptr& other) noexcept
		{
			using std::swap;
			swap(ptr_, other.ptr_);
		}

	public:
		constexpr unique_ptr() noexcept
			: ptr_()
		{}

		constexpr unique_ptr(nullptr_t) noexcept
			: ptr_(nullptr)
		{}

		constexpr unique_ptr(unique_ptr&& other) noexcept
			: ptr_(std::move(other.ptr_))
		{}

		unique_ptr& operator=(nullptr_t) noexcept
		{
			this->reset();
			return *this;
		}

		unique_ptr& operator=(unique_ptr&& other) noexcept
		{
			if (ptr_ != other.ptr_)
			{
				pointer old = other.ptr_;
				other.release();
				this->reset(old);
			}

			return *this;
		}

		explicit unique_ptr(pointer other) noexcept
			: ptr_(std::move(other))
		{}

		template <class OtherT, class OtherAddressT, process_rights OtherAccess>
		explicit unique_ptr(virtual_ptr<OtherT, OtherAddressT, OtherAccess> other) noexcept
			: ptr_(std::move(other))
		{}

		~unique_ptr() noexcept
		{
			if (ptr_)
				virtual_free(ptr_);
		}

		unique_ptr(const unique_ptr&) = delete;
		unique_ptr& operator=(const unique_ptr&) = delete;

	private:
		pointer ptr_;
	};

	template <class T, class AddressT, process_rights Access>
	unique_ptr<T, AddressT, Access>
		make_unique(process<Access>& process, address<AddressT> address = {}) noexcept
	{
		const auto memory_address = (address == memory::address<AddressT>{}) ? virtual_malloc(process, sizeof(T)) : address;
		return unique_ptr<T, AddressT, Access>{virtual_ptr<T, AddressT, Access>{process, memory_address}};
	}

	template <class T, process_rights Access>
	unique_ptr<T, dword, Access>
		make_unique(process<Access>& process, address<dword> address = {}) noexcept
	{
		const auto memory_address = (address == memory::address<dword>{}) ? virtual_malloc(process, sizeof(T)) : address;
		return unique_ptr<T, dword, Access>{virtual_ptr<T, dword, Access>{ process, memory_address }};
	}

	template <class Element, class Address, process_rights Access>
	void swap(unique_ptr<Element, Address, Access>& left, unique_ptr<Element, Address, Access>& right) noexcept
	{
		left.swap(right);
	}

} // namespace distant::memory
