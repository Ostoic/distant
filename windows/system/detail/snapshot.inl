#pragma once

#include <distant\windows\system\snapshot.hpp>

namespace distant::windows::system {

	template <class ObjectType>
	inline snapshot<ObjectType>::snapshot() 
		: m_handle(system::detail::get_snapshot<object_type, snapshot>())
	{
		static_assert(
			windows::is_kernel_object<ObjectType>::value,
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
	inline typename snapshot<ObjectType>::const_iterator 
	snapshot<ObjectType>::cbegin() const
	{
		return const_iterator(*this);
	}

	template <class ObjectType>
	inline typename snapshot<ObjectType>::const_iterator 
	snapshot<ObjectType>::cend() const
	{
		return const_iterator(*this, iterator::snapshot_end());
	}

} // end namespace distant::windows::system