#pragma once

#include <stdexcept>

#include <distant\system\snapshot.hpp>

namespace distant::system {

	template <class ObjectType>
	inline snapshot<ObjectType>::snapshot() 
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

	template <class ObjectType>
	inline typename snapshot<ObjectType>::iterator
	snapshot<ObjectType>::begin() const
	{
		return iterator(*this);
	}

	template <class ObjectType>
	inline typename snapshot<ObjectType>::iterator
	snapshot<ObjectType>::end() const
	{
		return iterator(*this, iterator::snapshot_end());
	}

} // end namespace distant::system
