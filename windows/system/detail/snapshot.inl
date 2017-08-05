#pragma once

#include <distant\windows\system\snapshot.hpp>

namespace distant::windows::system {

	template <class ObjectType>
	inline snapshot<ObjectType>::snapshot() : m_handle(system::detail::get_snapshot<object_type, snapshot>())
	{
		static_assert(
			windows::is_kernel_object<ObjectType>::value,
			"Invalid snapshot type (snapshot::ctor): "
			"Unable to take system snapshot of nonkernel object");

		this->update_gle();
	}

} // end namespace distant::windows::system