// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once
#include "../snapshot.hpp"

#include <distant/agents/interface.hpp>

namespace distant::agents {

	/// @brief Construct a snapshot of \a Agents owned by the given process.
	template <typename O>
	template <typename Ow>
	snapshot<O>::snapshot(const Ow& owner)
		: base(owner)
	{}

	template <typename O>
	template <template <typename, typename> class OutContainer>
	OutContainer<O, std::allocator<O>> snapshot<O>::as() const
	{
		OutContainer<O, std::allocator<O>> output;
		output.reserve(200);
		std::copy(this->begin(), this->end(), std::back_inserter(output));

		return output;
	}

	template <typename O>
	template <template <typename, typename> class OutContainer, typename Predicate>
	OutContainer<O, std::allocator<O>> snapshot<O>::as(Predicate predicate) const
	{
		OutContainer<O, std::allocator<O>> output;
		output.reserve(200);
		std::copy_if(this->begin(), this->end(), std::back_inserter(output), predicate);

		return output;
	}

} // namespace distant::system
