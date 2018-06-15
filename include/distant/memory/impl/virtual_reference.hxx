// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once
#include "../virtual_reference.hpp"

#include <distant/memory/virtual_memory.hpp>

namespace distant::memory
{
	template <typename E,  typename T, process_rights R>
	virtual_reference<E, T, R>::virtual_reference(pointer ptr) noexcept
		: ptr_(ptr) 
	{}

	template <typename E,  typename T, process_rights R>
	template <typename OE, typename OT, process_rights OR>
	virtual_reference<E, T, R>::virtual_reference(virtual_reference<OE, OT, OR> other) noexcept
		: ptr_(other.ptr_)
	{}

	template <typename E,  typename T, process_rights R>
	template <typename OE, typename OT, process_rights OR>
	virtual_reference<E, T, R>& virtual_reference<E, T, R>::operator=(virtual_reference<OE, OT, OR> other) noexcept
	{
		this->ptr_ = other.ptr_;
		return *this;
	}

	template <typename E,  typename T, process_rights R>
	template <typename Value, typename>
	virtual_reference<E, T, R>& virtual_reference<E, T, R>::operator=(const Value& x)
	{
		static_assert(
			R >= vm_w_op, 
			"[virtual_reference::operator value_type] The specified process rights do not permit writing of virtual memory."
		);

		memory::write<std::remove_cv_t<value_type>, T>(
			*(this->ptr_.process_), 
			this->ptr_.address_, 
			static_cast<value_type>(x)
		);

		return *this;
	}

	template <typename E,  typename T, process_rights R>
	typename virtual_reference<E, T, R>::pointer 
	virtual_reference<E, T, R>::operator&() const noexcept
	{
		return this->ptr_;
	}

	template <typename E,  typename T, process_rights R>
	virtual_reference<E, T, R>::operator value_type() const
	{
		static_assert(
			R >= process_rights::vm_read, 
			"[virtual_reference::operator value_type] The specified process rights do not permit reading of virtual memory."
		);

		return memory::read<std::remove_cv_t<value_type>>(*(this->ptr_.process_), this->ptr_.address_);
	}

	template <typename E,  typename T, process_rights R>
	void virtual_reference<E, T, R>::swap(virtual_reference& other)
	{
		value_type temp = *this;
		*this = other;
		return *this;
	}

	template <typename E,  typename T, process_rights R>
	virtual_reference<E, T, R>& virtual_reference<E, T, R>::operator++()
	{
		value_type temp = *this;
		++temp;
		*this = temp;
		return *this;
	}

	template <typename E,  typename T, process_rights R>
	typename virtual_reference<E, T, R>::value_type
	virtual_reference<E, T, R>::operator++(int)
	{
		value_type temp = *this;
		value_type result = temp++;
		*this = temp;
		return result;
	}

	template <typename E,  typename T, process_rights R>
	virtual_reference<E, T, R>& virtual_reference<E, T, R>::operator+=(const value_type& rhs)
	{
		value_type temp = *this;
		temp += rhs;
		*this = temp;
		return *this;
	}

	template <typename E,  typename T, process_rights R>
	virtual_reference<E, T, R>& virtual_reference<E, T, R>::operator--()
	{
		value_type temp = *this;
		--temp;
		return *this;
	}

	template <typename E,  typename T, process_rights R>
	typename virtual_reference<E, T, R>::value_type
	virtual_reference<E, T, R>::operator--(int)
	{
		value_type temp = *this;
		value_type result = temp++;
		*this = temp;
		return result;
	}

	template <typename E,  typename T, process_rights R>
	virtual_reference<E, T, R>& virtual_reference<E, T, R>::operator-=(const value_type& rhs)
	{
		value_type temp = *this;
		temp -= rhs;
		*this = temp;
		return *this;
	}

	template <typename E,  typename T, process_rights R>
	virtual_reference<E, T, R>& virtual_reference<E, T, R>::operator*=(const value_type& rhs)
	{
		value_type temp = *this;
		temp *= rhs;

		*this = temp;
		return *this;
	}

	template <typename E,  typename T, process_rights R>
	virtual_reference<E, T, R>& virtual_reference<E, T, R>::operator/=(const value_type& rhs)
	{
		value_type temp = *this;
		temp /= rhs;

		*this = temp;
		return *this;
	}

	template <typename E,  typename T, process_rights R>
	virtual_reference<E, T, R>& virtual_reference<E, T, R>::operator%=(const value_type& rhs)
	{
		value_type temp = *this;
		temp %= rhs;

		*this = temp;
		return *this;
	}

	template <typename E,  typename T, process_rights R>
	virtual_reference<E, T, R>& virtual_reference<E, T, R>::operator<<=(const value_type& rhs)
	{
		value_type temp = *this;
		temp <<= rhs;

		*this = temp;
		return *this;
	}

	template <typename E,  typename T, process_rights R>
	virtual_reference<E, T, R>& virtual_reference<E, T, R>::operator>>=(const value_type& rhs)
	{
		value_type temp = *this;
		temp >>= rhs;

		*this = temp;
		return *this;
	}

	template <typename E,  typename T, process_rights R>
	virtual_reference<E, T, R>& virtual_reference<E, T, R>::operator&=(const value_type& rhs)
	{
		value_type temp = *this;
		temp &= rhs;

		*this = temp;
		return *this;
	}

	template <typename E,  typename T, process_rights R>
	virtual_reference<E, T, R>& virtual_reference<E, T, R>::operator|=(const value_type& rhs)
	{
		value_type temp = *this;
		temp |= rhs;

		*this = temp;
		return *this;
	}

	template <typename E,  typename T, process_rights R>
	virtual_reference<E, T, R>& virtual_reference<E, T, R>::operator^=(const value_type& rhs)
	{
		value_type temp = *this;
		temp ^= rhs;

		*this = temp;
		return *this;
	}
}
