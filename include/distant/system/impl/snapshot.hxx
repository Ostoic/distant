#pragma once
#include <distant\system\snapshot.hpp>

#include <stdexcept>

namespace distant::system {

	template <typename O>
	inline snapshot<O>::snapshot() 
		: m_handle(system::detail::get_snapshot_handle<object_type, snapshot>())
	{
		static_assert(
			is_kernel_object<O>::value,
			"[snapshot::{ctor}] Unable to take system snapshot of nonkernel object");

		if (m_handle == nullptr)
			throw std::system_error(error::last_error(), "[snapshot::{ctor}] Invalid handle");
	}

	template <typename O>
	inline typename snapshot<O>::iterator
	snapshot<O>::begin() const
	{
		return iterator{*this};
	}

	template <typename O>
	inline typename snapshot<O>::iterator
	snapshot<O>::end() const
	{
		return iterator{*this, iterator::snapshot_end()};
	}

	template <typename O>
	template <typename OutContainer>
	OutContainer snapshot<O>::get() const
	{
		OutContainer output;
		std::copy_if(this->begin(), this->end(), std::back_inserter(output), [](auto&& element)
		{
			return std::move(element);
		});
		return output;
	}

	template <typename O>
	template <typename Predicate, typename OutContainer>
	OutContainer snapshot<O>:: get(Predicate predicate) const
	{
		OutContainer output;
		for (auto&& element : *this)
			if (element && predicate(element))
				output.push_back(std::move(element));

		return output;
	}

} // end namespace distant::system
