#pragma once

#include <Windows.h>
#include <type_traits>
#include <cstdint>

#include <distant\memory\address.h>
#include <distant\memory\pointer.h>
#include <distant\process.h>

#include <distant\windows\error\gle.h>

//#include <distant\iterators\vm_iterator.h>

// https://msdn.microsoft.com/en-us/library/windows/desktop/aa366525(v=vs.85).aspx

namespace distant {
namespace memory  {
	// Forward declare memory::pointer
	template <typename T>
	class pointer;

	// A vm is a view into a process' virtual memory.
	// It provides tools for manipulating and querying the memory of a process.
	class vm : public windows::gle
	{
	public:
		// Forward declare virtual memory operations
		class operation;
		class allocation;
		class deletion;

	public:
		using size_type	   = std::size_t;
		using address_type = memory::address;

	public:
		//vm() : m_process() {}
		vm(const process& p) : m_process(p){}

		//template <typename... Ts>
		//operation operate(Ts... args);

		//// Pass everything to the allocation ctor
		//template <typename... Ts>
		//allocation allocate(Ts... args);

		//// Pass everything to the allocation ctor
		//template <typename... Ts>
		//deletion deallocate(Ts... args);

		// Mutates: gle
		// Returns: Result of read
		template <typename T>
		T read(address_type address) const
		{
			return this->read<T>(address, sizeof(T));
		}

		// Mutates: gle
		// Returns: Result of read
		template <typename T>
		T read(address_type address, size_type bytes_to_read) const
		{
			static_assert(
				std::is_arithmetic_v<T>,
				"Type must satisfy std::is_arithmetic");

			T result = T();
			SIZE_T bytes_read = 0;

			// Ensure we have the correct access rights to perform the vm read.
			if (m_process.check_permission(process::access_rights::vm_read))
			{
				// reinterpret_cast because of WINAPI's general-purpose address pointer
				ReadProcessMemory(m_process, reinterpret_cast<LPVOID>(address), &result, bytes_to_read, &bytes_read);

				// Save the gle for this operation
				this->update_gle();
			}

			return result;
		}

		// Mutates: gle
		// Returns: Number of bytes written
		template <typename T>
		size_type write(address_type address, const T& to_write) const
		{
			return this->write<T>(address, to_write, sizeof(T));
		}

		// Mutates: gle
		// Returns: Number of bytes written
		template <typename T>
		size_type write(address_type address, const T& to_write, size_type bytes_to_write) const
		{
			static_assert(
				std::is_arithmetic_v<T>,
				"Type must satisfy std::is_arithmetic");

			SIZE_T bytes_written = 0;

			T buffer = to_write;

			// Ensure we have the correct access rights to perform the vm write.
			if (m_process.check_permission(process::access_rights::vm_write))
			{
				// reinterpret_cast because of WINAPI's general-purpose address pointer
				WriteProcessMemory(m_process, reinterpret_cast<LPVOID>(address), &buffer, bytes_to_write, &bytes_written);
			
				// Save the gle for this operation
				this->update_gle();
			}

			return static_cast<size_type>(bytes_written);
		}

		// Return pointer to memory address in vm.
		// This pointer acts as a regular pointer that is local to the current
		// process.
		// XXX Rename to make_ptr
		template <typename T>
		pointer<T> ptr(address_type address) { return pointer<T>(*this, address); }

		friend bool operator ==(const vm& lhs, const vm& rhs);
		friend bool operator !=(const vm& lhs, const vm& rhs);

	protected:
		const process& m_process;
	};

	inline bool operator ==(const vm& lhs, const vm& rhs)
	{
		return lhs.m_process == rhs.m_process;
	}

	inline bool operator !=(const vm& lhs, const vm& rhs)
	{
		return !operator==(lhs, rhs);
	}

} // end namespace memory
} // end namespace distant

// Include vm::operation implementation
#include <distant\memory\vm\allocation.inl>
