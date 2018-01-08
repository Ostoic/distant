#pragma once

#include <distant\support\filesystem.hpp>

#include <string>

namespace distant {
namespace system  {

	const filesystem::path& windows_path();

	const std::wstring& computer_name();

	const std::wstring& username();

	std::size_t processor_type() noexcept;

	std::size_t page_size() noexcept;

	std::size_t allocation_granularity() noexcept;

	std::size_t number_of_processors() noexcept;

	std::size_t oem_id() noexcept;

#if BOOST_USE_WINAPI_VERSION >= BOOST_WINAPI_VERSION_WIN8
	bool is_virtual_machine() noexcept;
#endif // win8 version check

} // end namespace system
} // end namespace distant

// Implementation:
#include <distant\system\impl\information.hxx>
