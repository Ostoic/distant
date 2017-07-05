#pragma once

#include <distant\memory\address.h>
#include <distant\memory\vm.h>

namespace distant {
namespace memory  {

	// Pointer with mutable address
	template <typename T>
	class value
	{
	public:
		using address_type = memory::address_type;
		using value_type   = T;

	public:
		value() : m_vm(), m_address() {}

		value(T&& val, address_type address) : m_address(address) { using std::swap; swap(m_val, val); }
		value(const T& val, address_type address) : m_value(val), m_address(address) {}
		value(const vm& v, address_type address) : m_address(address), m_vm(v) {}

		template <typename U>
		friend bool operator ==(const value<U>&, const value<U>&);
		template <typename U>
		friend bool operator !=(const value<U>&, const value<U>&);

		operator const T&() const { return m_value; }

		value& operator =(const T& val)
		{
			m_vm.write(val, m_address);
			m_value = val;
		}

	private:
		T m_value;
		address_type m_address;

		const vm& m_vm;
	};

	template <typename T>
	friend bool operator ==(const value<T>& lhs, const value<T>& rhs)
	{ 
		return lhs.m_address == rhs.m_address &&
			   lhs.m_vm == rhs.m_vm			  &&
			   lhs.m_value == rhs.m_value;
	}

	template <typename T>
	friend bool operator !=(const value<T>& lhs, const value<T>& rhs)
	{ return !operator==(lhs, rhs); }

} // end namespace memory
} // end namespace distant