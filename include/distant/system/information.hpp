// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/support/filesystem.hpp>
#include <distant/system/architectures.hpp>

#include <string>

namespace distant::system {

	const filesystem::path& windows_path();

	const filesystem::path& system_path();

	std::wstring computer_name();

	std::wstring username();

	uint processor_type() noexcept;

	std::size_t page_size() noexcept;

	std::size_t allocation_granularity() noexcept;

	uint number_of_processors() noexcept;

	processor_architecture architecture() noexcept;

#if BOOST_USE_WINAPI_VERSION >= BOOST_WINAPI_VERSION_WIN8
	bool is_virtual_machine() noexcept;
#endif // win8 version check
} // end namespace distant::system

// Implementation:
#include <distant/system/impl/information.hxx>
