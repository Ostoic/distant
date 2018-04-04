#pragma once
#include "../virtual_reference.hpp"

#include <distant/memory/virtual_memory.hpp>

namespace distant::memory
{
	template <typename E,  typename T>
	virtual_reference<E, T>::virtual_reference(pointer ptr)
		: ptr_(ptr) 
	{}

	template <typename E,  typename T>
	template <typename OE, typename OT>
	virtual_reference<E, T>::virtual_reference(virtual_reference<OE, OT> other)
		: ptr_(other.ptr_)
	{}

	template <typename E,  typename T>
	template <typename OE, typename OT>
	virtual_reference<E, T>& virtual_reference<E, T>::operator=(virtual_reference<OE, OT> other)
	{
		this->ptr_ = other.ptr_;
		return *this;
	}

	template <typename E,  typename T>
	virtual_reference<E, T>& virtual_reference<E, T>::operator=(const value_type& x)
	{
		static_assert(
			!std::is_const<E>::value,
			"[virtual_reference::operator=] Cannot assign to constant value"
		);

		memory::write<value_type, T>(*this->ptr_.process_, this->ptr_.address_, x);
		return *this;
	}

	template <typename E,  typename T>
	typename virtual_reference<E, T>::pointer 
	virtual_reference<E, T>::operator&() const
	{
		return this->ptr_;
	}

	template <typename E,  typename T>
	virtual_reference<E, T>::operator value_type() const
	{
		return memory::read<std::remove_cv_t<value_type>>(*this->ptr_.process_, this->ptr_.address_);
	}

	template <typename E,  typename T>
	void virtual_reference<E, T>::swap(virtual_reference& other)
	{
		value_type temp = *this;
		*this = other;
		return *this;
	}

	template <typename E,  typename T>
	virtual_reference<E, T>& virtual_reference<E, T>::operator++()
	{
		value_type temp = *this;
		++temp;
		*this = temp;
		return *this;
	}

	template <typename E,  typename T>
	typename virtual_reference<E, T>::value_type
	virtual_reference<E, T>::operator++(int)
	{
		value_type temp = *this;
		value_type result = temp++;
		*this = temp;
		return result;
	}

	template <typename E,  typename T>
	virtual_reference<E, T>& virtual_reference<E, T>::operator+=(const value_type& rhs)
	{
		value_type temp = *this;
		temp += rhs;
		*this = temp;
		return *this;
	}

	template <typename E,  typename T>
	virtual_reference<E, T>& virtual_reference<E, T>::operator--()
	{
		value_type temp = *this;
		--temp;
		return *this;
	}

	template <typename E,  typename T>
	typename virtual_reference<E, T>::value_type
	virtual_reference<E, T>::operator--(int)
	{
		value_type temp = *this;
		value_type result = temp++;
		*this = temp;
		return result;
	}

	template <typename E,  typename T>
	virtual_reference<E, T>& virtual_reference<E, T>::operator-=(const value_type& rhs)
	{
		value_type temp = *this;
		temp -= rhs;
		*this = temp;
		return *this;
	}

	template <typename E,  typename T>
	virtual_reference<E, T>& virtual_reference<E, T>::operator*=(const value_type& rhs)
	{
		value_type temp = *this;
		temp *= rhs;

		*this = temp;
		return *this;
	}

	template <typename E,  typename T>
	virtual_reference<E, T>& virtual_reference<E, T>::operator/=(const value_type& rhs)
	{
		value_type temp = *this;
		temp /= rhs;

		*this = temp;
		return *this;
	}

	template <typename E,  typename T>
	virtual_reference<E, T>& virtual_reference<E, T>::operator%=(const value_type& rhs)
	{
		value_type temp = *this;
		temp %= rhs;

		*this = temp;
		return *this;
	}

	template <typename E,  typename T>
	virtual_reference<E, T>& virtual_reference<E, T>::operator<<=(const value_type& rhs)
	{
		value_type temp = *this;
		temp <<= rhs;

		*this = temp;
		return *this;
	}

	template <typename E,  typename T>
	virtual_reference<E, T>& virtual_reference<E, T>::operator>>=(const value_type& rhs)
	{
		value_type temp = *this;
		temp >>= rhs;

		*this = temp;
		return *this;
	}

	template <typename E,  typename T>
	virtual_reference<E, T>& virtual_reference<E, T>::operator&=(const value_type& rhs)
	{
		value_type temp = *this;
		temp &= rhs;

		*this = temp;
		return *this;
	}

	template <typename E,  typename T>
	virtual_reference<E, T>& virtual_reference<E, T>::operator|=(const value_type& rhs)
	{
		value_type temp = *this;
		temp |= rhs;

		*this = temp;
		return *this;
	}

	template <typename E,  typename T>
	virtual_reference<E, T>& virtual_reference<E, T>::operator^=(const value_type& rhs)
	{
		value_type temp = *this;
		temp ^= rhs;

		*this = temp;
		return *this;
	}
}
