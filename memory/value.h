#pragma once

#include <distant\memory\address.h>
#include <distant\memory\vm.h>

namespace distant {
namespace memory  {

	// Forward declare memory::vm
	class vm;

	// Pointer with mutable address
	template <typename T>
	class value
	{
	public:
		using address_type = memory::address_type;
		using value_type   = T;

	public:
		value() : m_vm(), m_address() {}

		value(vm& v, address_type address, const value_type& val) :
			m_vm(v),
			//m_value(val),
			m_address(address) 
		{ m_vm.write<value_type>(m_address, val); }

		value(vm& v, address_type address) : 
			m_address(address), 
			m_vm(v) 
		{}

		template <typename U>
		friend bool operator ==(const value<U>&, const value<U>&);
		template <typename U>
		friend bool operator !=(const value<U>&, const value<U>&);

		operator T() const { return m_vm.read<value_type>(m_address); }

		value& operator =(value_type val)
		{
			m_vm.write<value_type>(m_address, val);
			//m_value = m_vm.read<value_type>(m_address);
			return *this;
		}

	private:
		//value_type m_value;
		address_type m_address;

		vm& m_vm;
	};

	template <typename T>
	inline bool operator ==(const value<T>& lhs, const value<T>& rhs)
	{ 
		return lhs.m_address == rhs.m_address &&
			   lhs.m_vm == rhs.m_vm;		  
			   //lhs.m_value == rhs.m_value;
	}

	template <typename T>
	inline bool operator !=(const value<T>& lhs, const value<T>& rhs)
	{ return !operator==(lhs, rhs); }

} // end namespace memory
} // end namespace distant