// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <utility>
#include <boost/mp11/list.hpp>
#include <boost/fusion/container.hpp>

namespace distant::assembly
{
	class static_instruction
	{
	private:
		using index_t = std::size_t;
		using length_t = std::size_t;

	public:
		constexpr std::size_t size() const noexcept;

	private:
		//boost::mp11::mp_list<
		//boost::fusion::make_vector()
	};
}

// Implementation:
#include <distant/assembly/impl/static_instruction.hxx>
