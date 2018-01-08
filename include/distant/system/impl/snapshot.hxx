#pragma once
#include <distant\system\snapshot.hpp>

#include <stdexcept>

namespace distant::system {

	template <typename O, typename C>
	inline snapshot<O, C>::snapshot() 
		: m_handle(system::detail::get_snapshot_handle<object_type, snapshot>())
	{
		static_assert(
			is_kernel_object<O>::value,
			"Invalid template parameter (snapshot::ctor): "
			"Unable to take system snapshot of nonkernel object");

		if (m_handle == distant::invalid_handle)
			throw std::system_error(error::last_error(), "[snapshot::{ctor}] invalid handle");
	}

	template <typename O, typename C>
	inline typename snapshot<O, C>::iterator
	snapshot<O, C>::begin() const
	{
		return iterator(*this);
	}

	template <typename O, typename C>
	inline typename snapshot<O, C>::iterator
	snapshot<O, C>::end() const
	{
		return iterator(*this, iterator::snapshot_end());
	}

	template <typename O, typename C>
	typename snapshot<O, C>::output_type 
	snapshot<O, C>::get() const
	{
		output_type output;
		std::copy(this->begin(), this->end(), std::back_inserter(output));
		return output;
	}
/*
	template <class O, class C>
	operator typename snapshot<O, C>::output_type() const
	{
		return this->get();
	}*/

} // end namespace distant::system
