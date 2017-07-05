#pragma once

#include <Windows.h>
#include <type_traits>
#include <cstdint>

#include <distant\process\process.h>
#include <distant\memory\address.h>
//#include <distant\iterators\vm_iterator.h>

namespace distant {

	// Forward delcare process
	class process;

namespace memory  {

	// A vm is a view into a process' virtual memory.
	// It provides tools for manipulating and querying the memory of a process.
	class vm
	{
	public:
		using size_type	   = std::size_t;
		using error_type   = DWORD;

	public:
		vm(const process& p) : m_process(p){}
		
		// Mutates: m_error
		template <typename T>
		T read(address_type address)
		{
			T result = this->read<T>(sizeof(T));
			return result;
		}

		// Mutates: m_error
		template <typename T>
		T read(address_type address, size_type bytes_to_read)
		{
			T result;
			SIZE_T bytes_read = 0;

			// Ensure we have the correct access rights to perform the vm read.
			if (m_process.check_permission(process::access_rights::vm_read))
				// reinterpret_cast because of WINAPI's general-purpose address pointer
				ReadProcessMemory(m_process, reinterpret_cast<LPVOID>(address), &result, bytes_to_read, &bytes_read);

			// Save last error for this operation
			m_error = GetLastError();
			return result;
		}

		// Mutates: m_error
		template <typename T>
		size_type write(address_type address, const T& value)
		{
			size_type bytes_written = this->write(address, value, sizeof(T));
			return bytes_written;
		}

		// Mutates: m_error
		template <typename T>
		size_type write(address_type address, T&& value)
		{
			size_type bytes_written = this->write(address, std::forward(value), sizeof(T));
			return bytes_written;
		}

		// Mutates: m_error
		template <typename T>
		size_type write(address_type address, T&& value, size_type bytes_to_write)
		{
			T buffer;

			std::swap(value, buffer);
			size_type bytes_written = this->write(address, buffer, bytes_to_write);
			return bytes_written;
		}

		// Mutates: m_error
		template <typename T>
		size_type write(address_type address, const T& value, size_type bytes_to_write)
		{
			SIZE_T bytes_written = 0;

			// Ensure we have the correct access rights to perform the vm write.
			if (m_process.check_permission(process::access_rights::vm_write))
				// reinterpret_cast because of WINAPI's general-purpose address pointer
				WriteProcessMemory(m_process, reinterpret_cast<LPVOID>(address), &value, bytes_to_write, &bytes_written);

			// Save last error for this operation
			m_error = GetLastError();
			return static_cast<size_type>(bytes_written);
		}

		error_type get_last_error() const { return m_error; }

		friend bool operator ==(const vm& lhs, const vm& rhs);
		friend bool operator !=(const vm& lhs, const vm& rhs);

	protected:
		const process& m_process;
		error_type m_error;
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