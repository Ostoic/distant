#pragma once

#include <distant/memory/address.hpp>
#include <distant/system/information.hpp>

namespace distant::memory
{
	/// @brief Represents a memory page in a process.
	template <class AddressT>
	class page
	{
	public:
		page(address<AddressT> address) noexcept
			: base_(address - (address % this->size()))
		{}

		address<AddressT> base() const noexcept { return base_; }
		static std::size_t size() noexcept { return system::page_size(); }

	private:
		address<AddressT> base_;
	};

} // namespace distant::memory
