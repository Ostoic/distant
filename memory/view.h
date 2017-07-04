#pragma once

#include <type_traits>

#include <Windows.h>
#include <distant\process\process.h>

namespace distant {
namespace memory  {

	// A view is a view into a process' virtual memory.
	// It provides tools for manipulating and querying the memory of a process.
	class view
	{
	public:
		using size_type	   = std::size_t;
		using error_type   = DWORD;
		using address_type = DWORD;

	public:
		view(const process& p) : m_process(p) {}
		
		template <typename T>
		T read(address_type addr)
		{
			T result = this->read(addr, sizeof(T));
			return result;
		}

		template <typename T>
		T read(address_type addr, size_type bytes_to_read)
		{
			T result;
			size_type bytes_read = 0;

			// Ensure we have the correct access rights to perform the vm read.
			if (m_process.check_permission(process::access_rights::vm_read))
				ReadProcessMemory(m_process, addr, &result, bytes_to_read, &bytes_read);

			// Save last error for this operation
			m_error = GetLastError();
			return result;
		}

		template <typename T>
		size_type write(address_type addr, const T& value)
		{
			size_type bytes_written = this->write(addr, value, sizeof(T));
			return bytes_written;
		}

		template <typename T>
		size_type write(address_type addr, T&& value)
		{
			size_type bytes_written = this->write(addr, std::forward(value), sizeof(T));
			return bytes_written;
		}

		template <typename T>
		size_type write(address_type addr, T&& value, size_type bytes_to_write)
		{
			T buffer;

			std::swap(value, buffer);
			size_type bytes_written = this->write(addr, buffer, bytes_to_write);
			return bytes_written;
		}

		template <typename T>
		size_type write(address_type addr, const T& value, size_type bytes_to_write)
		{
			size_type bytes_written = 0;

			// Ensure we have the correct access rights to perform the vm write.
			if (m_process.check_permission(process::access_rights::vm_write))
				WriteProcessMemory(m_process, addr, &value, bytes_to_write, &bytes_written);

			// Save last error for this operation
			m_error = GetLastError();
			return bytes_written;
		}

		error_type get_last_error() const { return m_error; }

	private:
		const process& m_process;
		error_type m_error;
	};

} // end namespace memory
} // end namespace distant