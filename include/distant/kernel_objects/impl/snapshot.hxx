// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once
#include <distant/kernel_objects/snapshot.hpp>

#include <stdexcept>

namespace distant::kernel_objects {

	template <typename O>
	snapshot<O>::snapshot()
		: base(snapshot_traits::create_snapshot_handle())
	{
		if (handle_ == nullptr)
			throw windows_error("[snapshot::{ctor}] Invalid handle");
	}

	/// @brief Construct a snapshot of \a KernelObjects owned by the given process.
	template <typename O>
	template <typename Ow>
	snapshot<O>::snapshot(const Ow& owner)
		: base(snapshot_traits::create_snapshot_handle())
	{}

	template <typename O>
	typename snapshot<O>::const_iterator snapshot<O>
		::begin() const
	{ return iterator{*this}; }

	template <typename O>
	typename snapshot<O>::const_iterator snapshot<O>
		::end() const
	{ return iterator{}; }

	template <typename O>
	typename snapshot<O>::iterator snapshot<O>
		::begin()
	{ return iterator{ *this }; }

	template <typename O>
	typename snapshot<O>::iterator snapshot<O>
		::end()
	{ return iterator{}; }

	template <typename O>
	template <template <typename, typename> class OutContainer>
	OutContainer<O, std::allocator<O>> snapshot<O>::as() const
	{
		OutContainer<O, std::allocator<O>> output;
		std::copy_if(this->begin(), this->end(), std::back_inserter(output), [](auto&& element)
		{
			return element;
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
				output.emplace_back(std::move(element));

		return output;
	}

} // namespace distant::system
