#pragma once
#include "../virtual_reference.hpp"

#include <distant/memory/virtual_memory.hpp>

namespace distant::memory
{
	template <typename E, process_rights A, typename T>
	virtual_reference<E, A, T>::virtual_reference(const pointer& ptr)
		: ptr_(ptr) 
	{}

	template <typename E, process_rights A, typename T>
	template <typename OE, process_rights OA, typename OT>
	virtual_reference<E, A, T>::virtual_reference(const virtual_reference<OE, OA, OT>& other)
		: ptr_(other.ptr_)
	{
		static_assert(
			check_permission(A, OA),
			"[virtual_reference::{ctor}] Process access rights must be interoparable"
		);
	}


	template <typename E, process_rights A, typename T>
	virtual_reference<E, A, T>& virtual_reference<E, A, T>::operator=(const virtual_reference& other)
	{
		value_type temp = other;
		*this = temp;
		return *this;
	}

	template <typename E, process_rights A, typename T>
	template <typename OE, process_rights OA, typename OT>
	virtual_reference<E, A, T>& virtual_reference<E, A, T>::operator=(const virtual_reference<OE, OA, OT>& other)
	{
		static_assert(
			check_permission(A, OA),
			"[virtual_reference::{ctor}] Process access rights must be interoparable"
		);

		this->ptr_ = other.ptr_;
		return *this;
	}

	template <typename E, process_rights A, typename T>
	virtual_reference<E, A, T>& virtual_reference<E, A, T>::operator=(const value_type& x)
	{
		memory::write<value_type, T>(*this->ptr_.process_, this->ptr_.address_, x);
		return *this;
	}

	template <typename E, process_rights A, typename T>
	typename virtual_reference<E, A, T>::pointer 
	virtual_reference<E, A, T>::operator&() const
	{
		return this->ptr_;
	}

	template <typename E, process_rights A, typename T>
	virtual_reference<E, A, T>::operator value_type() const
	{
		return memory::read<value_type>(*this->ptr_.process_, this->ptr_.address_);
	}

	template <typename E, process_rights A, typename T>
	void virtual_reference<E, A, T>::swap(virtual_reference& other)
	{
		value_type temp = *this;
		*this = other;
		return *this;
	}

	template <typename E, process_rights A, typename T>
	virtual_reference<E, A, T>& virtual_reference<E, A, T>::operator++()
	{
		value_type temp = *this;
		++temp;
		*this = temp;
		return *this;
	}

	template <typename E, process_rights A, typename T>
	typename virtual_reference<E, A, T>::value_type
	virtual_reference<E, A, T>::operator++(int)
	{
		value_type temp = *this;
		value_type result = temp++;
		*this = temp;
		return result;
	}

	template <typename E, process_rights A, typename T>
	virtual_reference<E, A, T>& virtual_reference<E, A, T>::operator+=(const value_type& rhs)
	{
		value_type temp = *this;
		temp += rhs;
		*this = temp
		return *this;
	}

	template <typename E, process_rights A, typename T>
	virtual_reference<E, A, T>& virtual_reference<E, A, T>::operator--()
	{
		value_type temp = *this;
		--temp;
		return *this;
	}

	template <typename E, process_rights A, typename T>
	typename virtual_reference<E, A, T>::value_type
	virtual_reference<E, A, T>::operator--(int)
	{
		value_type temp = *this;
		value_type result = temp++;
		*this = temp;
		return result;
	}

	template <typename E, process_rights A, typename T>
	virtual_reference<E, A, T>& virtual_reference<E, A, T>::operator-=(const value_type& rhs)
	{
		value_type temp = *this;
		temp -= rhs;
		*this = temp;
		return *this;
	}

	template <typename E, process_rights A, typename T>
	virtual_reference<E, A, T>& virtual_reference<E, A, T>::operator*=(const value_type& rhs)
	{
		value_type temp = *this;
		temp *= rhs;

		*this = temp;
		return *this;
	}

	template <typename E, process_rights A, typename T>
	virtual_reference<E, A, T>& virtual_reference<E, A, T>::operator/=(const value_type& rhs)
	{
		value_type temp = *this;
		temp /= rhs;

		*this = temp;
		return *this;
	}

	template <typename E, process_rights A, typename T>
	virtual_reference<E, A, T>& virtual_reference<E, A, T>::operator%=(const value_type& rhs)
	{
		value_type temp = *this;
		temp %= rhs;

		*this = temp;
		return *this;
	}

	template <typename E, process_rights A, typename T>
	virtual_reference<E, A, T>& virtual_reference<E, A, T>::operator<<=(const value_type& rhs)
	{
		value_type temp = *this;
		temp <<= rhs;

		*this = temp;
		return *this;
	}

	template <typename E, process_rights A, typename T>
	virtual_reference<E, A, T>& virtual_reference<E, A, T>::operator>>=(const value_type& rhs)
	{
		value_type temp = *this;
		temp >>= rhs;

		*this = temp;
		return *this;
	}

	template <typename E, process_rights A, typename T>
	virtual_reference<E, A, T>& virtual_reference<E, A, T>::operator&=(const value_type& rhs)
	{
		value_type temp = *this;
		temp &= rhs;

		*this = temp;
		return *this;
	}

	template <typename E, process_rights A, typename T>
	virtual_reference<E, A, T>& virtual_reference<E, A, T>::operator|=(const value_type& rhs)
	{
		value_type temp = *this;
		temp |= rhs;

		*this = temp;
		return *this;
	}

	template <typename E, process_rights A, typename T>
	virtual_reference<E, A, T>& virtual_reference<E, A, T>::operator^=(const value_type& rhs)
	{
		value_type temp = *this;
		temp ^= rhs;

		*this = temp;
		return *this;
	}
}
