#pragma once

#include <stdexcept>

#include <distant\system\snapshot.hpp>

namespace distant::system {

	template <class ObjectType>
	inline snapshot<ObjectType>::snapshot() 
		: m_handle(system::detail::get_snapshot<object_type, snapshot>())
	{
		static_assert(
			is_kernel_object<ObjectType>::value,
			"Invalid template parameter (snapshot::ctor): "
			"Unable to take system snapshot of nonkernel object");

		this->update_gle();
		if (m_handle == distant::invalid_handle)
		{
			std::error_code error(this->get_error_code(), std::system_category());
			throw std::system_error(error, "Invalid snapshot handle");
		}
	}
	
	template <class ObjectType>
	inline typename snapshot<ObjectType>::iterator
	snapshot<ObjectType>::begin()
	{
		return iterator(*this);
	}

	template <class ObjectType>
	inline typename snapshot<ObjectType>::iterator
	snapshot<ObjectType>::end()
	{
		return iterator(*this, iterator::snapshot_end());
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
