#pragma once

#include <Windows.h>
#include <type_traits>
#include <cstdint>

#include <distant\process\process.h>
#include <distant\memory\address.h>
#include <distant\memory\vm.h>

namespace distant {

	// Forward delcare process
	class process;

namespace memory  {

	template <typename T>
	class view : private vm	
	{
	public:
		using size_type	   = std::size_t;
		using error_type   = DWORD;

	public:
		view(const process& p, address_type address) : vm(p), m_address(address) {}
		
		// Note that RVO should ellide all copies below
		/**************************************/
		/*** memory::vm operation delegates ***/
		/**************************************/

		// Mutates: m_error
		// vm::read delegate
		T read() { return vm::read<T>(m_address); }

		// Mutates: m_error
		// vm::read delegate
		T read(size_type bytes_to_read) { return vm::read<T>(m_address, bytes_to_read); }

		// Mutates: m_error
		// vm::write delegate
		size_type write(const T& value) { return vm::write(m_address, value, sizeof(T)); }

		// Mutates: m_error
		// vm::write delegate
		size_type write(T&& value) { return vm::write(m_address, std::forward(value), sizeof(T)); }

		// Mutates: m_error
		// vm::write delegate
		size_type write(T&& value, size_type bytes_to_write) { return vm::write(m_address, std::forward(buffer), bytes_to_write); }

		// Mutates: m_error
		// vm::write delegate
		size_type write(const T& value, size_type bytes_to_write) { return vm::write(m_address, value, bytes_to_write); }

		using vm::get_last_error;

		template <typename U>
		friend bool operator ==(const view<U>& lhs, const view<U>& rhs);

		template <typename U>
		friend bool operator !=(const view<U>& lhs, const view<U>& rhs);

	protected:
		const address_type m_address;
	};

	template <typename U>
	inline bool operator ==(const view<U>& lhs, const view<U>& rhs)
	{
		return operator==(static_cast<vm>(lhs), static_cast<vm>(rhs)) &&
			   lhs.m_address == rhs.m_address;
	}

	template <typename U>
	inline bool operator !=(const view<U>& lhs, const view<U>& rhs)
	{
		return !operator==(lhs, rhs);
	}

} // end namespace memory
} // end namespace distant