#pragma once

#include <distant\support\filesystem.hpp>

namespace distant {
namespace system  {

	filesystem::path get_windows_path() noexcept;

} // end namespace system
} // end namespace distant

// Implementation:
#include <distant\system\impl\information.hxx>
