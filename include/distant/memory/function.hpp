// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/memory/address.hpp>

namespace distant::memory
{
	template <typename Signature, typename AddressT = dword>
	class function
	{
	public:
		function() noexcept = default;
		explicit function(memory::address<AddressT> address);

		memory::address<AddressT> address() const noexcept;

	private:
		memory::address<AddressT> address_;
	};

} // namespace distant::memory

#include "impl/function.hxx"