#pragma once

#include <stdexcept>

#include <distant\system\snapshot.hpp>

namespace distant::system {

	template <class O, class C>
	inline snapshot<O, C>::snapshot() 
		: object(system::detail::get_snapshot<object_type, snapshot>())
	{
		static_assert(
			is_kernel_object<ObjectType>::value,
			"Invalid template parameter (snapshot::ctor): "
			"Unable to take system snapshot of nonkernel object");

		m_last_error.update();
		if (m_handle == distant::invalid_handle)
			throw std::system_error(m_last_error, "Invalid snapshot handle");
	}

	template <class O, class C>
	inline typename snapshot<O, C>::iterator
	snapshot<O, C>::begin() const
	{
		return iterator(*this);
	}

	template <class O, class C>
	inline typename snapshot<O, C>::iterator
	snapshot<O, C>::end() const
	{
		return iterator(*this, iterator::snapshot_end());
	}

	template <class O, class C>
	typename snapshot<O, C>::output_type snapshot<O, C>::get() const
	{
		output_type output;
		std::copy(this->begin(), this->end(), std::back_inserter(output));
		return output;
	}

	template <class O, class C>
	operator typename snapshot<O, C>::output_type() const
	{
		return this->get();
	}


} // end namespace distant::system
