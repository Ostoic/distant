#pragma once

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