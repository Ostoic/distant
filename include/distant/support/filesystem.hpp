// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

/**
 * @file Configures the filesystem library used internally in the library.
 */

#pragma once

#include <distant/config.hpp>

#ifndef DISTANT_FILESYSTEM_CONFIG
#	define DISTANT_FILESYSTEM_CONFIG
#	ifdef __cpp_lib_filesystem
#		include <filesystem>
		namespace distant { namespace filesystem = std::filesystem; }
#	elif defined(__cpp_lib_experimental_filesystem)
#		include <experimental/filesystem>
		namespace distant { namespace filesystem = std::experimental::filesystem; }
#	else
#		include <boost/filesystem.hpp>
		namespace distant { namespace filesystem = boost::filesystem; }
#	endif
#endif
