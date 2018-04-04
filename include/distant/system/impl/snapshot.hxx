#pragma once
#include <distant/system/snapshot.hpp>

#include <stdexcept>

namespace distant::system {

	template <typename O>
	snapshot<O>::snapshot() 
		: handle_(system::detail::get_snapshot_handle<object_type, snapshot>())
	{
		static_assert(
			is_kernel_object<O>::value,
			"[snapshot::{ctor}] Unable to take system snapshot of nonkernel object");

		if (handle_ == nullptr)
			throw std::system_error(error::last_error(), "[snapshot::{ctor}] Invalid handle");
	}

	template <typename O>
	typename snapshot<O>::iterator
	snapshot<O>::begin() const
	{
		return iterator{*this};
	}

	template <typename O>
	typename snapshot<O>::iterator
	snapshot<O>::end() const
	{
		return iterator{*this, iterator::snapshot_end()};
	}

	template <typename O>
	template <template <typename, typename> class OutContainer>
	OutContainer<O, std::allocator<O>> snapshot<O>::as() const
	{
		OutContainer<O, std::allocator<O>> output;
		std::copy_if(this->begin(), this->end(), std::back_inserter(output), [](auto&& element)
		{
			return element.valid();
		});

		return output;
	}

	template <typename O>
	template <template <typename, typename> class OutContainer, typename Predicate>
	OutContainer<O, std::allocator<O>> snapshot<O>::as(Predicate predicate) const
	{
		OutContainer<O, std::allocator<O>> output;
		for (auto&& element : *this)
			if (element && predicate(element))
				output.push_back(std::move(element));

		return output;
	}
/*
	template <class O, class C>
	operator typename snapshot<O>::output_type() const
	{
		return this->as();
	}*/

} // end namespace distant::system
