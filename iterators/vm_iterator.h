#pragma once

#include <iterator>

#include <boost\iterator\iterator_adaptor.hpp>

#include <distant\process\process.h>
#include <distant\memory\address.h>
#include <distant\memory\pointer.h>
#include <distant\memory\vm.h>

namespace distant	{
namespace iterators {

template <typename T>
class vm_iterator :
	public boost::iterator_adaptor<vm_iterator<T>, // Derived
								   T, // Type
								   std::forward_iterator_tag, // Traversal 
								   T> // Reference
{
public:
	//view_iterator(): m_vm(vm), m_address(address) {}
	explicit vm_iterator(const memory::vm& vm, memory::address_type address) :
		m_vm(vm), 
		m_address(address) {}

private:
	friend class boost::iterator_core_access;

	void increment() 
	{
		m_address += sizeof(T);
	}

	bool equal(const vm_iterator& other) const
	{
		return m_address == other.m_address &&
			   m_vm		 == other.m_vm;

	}

	T dereference() { return m_vm.read<T>(m_address); }

private:
	const memory::vm& m_vm;
	memory::address_type m_address;
};

} // end namespace iterators
} // end namespace distant