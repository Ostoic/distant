#pragma once

#include <distant\memory\address.h>
#include <distant\memory\vm.h>

namespace distant {
namespace memory  {

	// Pointer with mutable address
	template <typename T>
	class pointer
	{
	public:
		using address_type = memory::address_type;
		using value_type   = T;

	public:
		pointer(const vm& v, address_type address) : m_address(address), m_vm(v) {}

		template <typename U>
		friend bool operator ==(const pointer<U>&, const pointer<U>&) const;
		template <typename U>
		friend bool operator !=(const pointer<U>&, const pointer<U>&) const;

		operator address_type&()	  { return m_address; }
		operator address_type() const { return m_address; }

		T operator *() { return m_vm.read<T>(m_address); }

		explicit operator bool() const { return m_address != 0; }

	private:
		address_type m_address;
		const vm& m_vm;
	};

	template <typename T>
	friend bool operator ==(const pointer<T>& lhs, const pointer<T>& rhs) const
	{ 
		return lhs.m_address == rhs.m_address &&
			   lhs.m_vm == rhs.m_vm;
	}

	template <typename T>
	friend bool operator !=(const pointer<T>& lhs, const pointer<T>& rhs) const
	{ return !operator==(lhs, rhs); }

	// Pointer with constant address
	template <typename T>
	class const_pointer
	{
	public:
		using address_type = memory::address_type;
		using value_type   = T;

	public:
		const_pointer(const vm& v, address_type address) : m_address(address), m_vm(v) {}

		template <typename U>
		friend bool operator ==(const const_pointer<U>&, const const_pointer<U>&);
		template <typename U>
		friend bool operator !=(const const_pointer<U>&, const const_pointer<U>&);

		operator address_type() const { return m_address; }

		explicit operator bool() { return m_address != 0; }

	private:
		const address_type m_address;
		const vm& m_vm;
	};

	template <typename T>
	friend bool operator ==(const const_pointer<T>& lhs, const const_pointer<T>& rhs) const
	{ 
		return lhs.m_address == rhs.m_address &&
			   lhs.m_vm == rhs.m_vm;
	}

	template <typename T>
	friend bool operator !=(const const_pointer<T>& lhs, const const_pointer<T>& rhs) const
	{ return !operator==(lhs, rhs); }

} // end namespace memory
} // end namespace distant