// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <boost/winapi/basic_types.hpp>

#if !defined (BOOST_USE_WINDOWS_H)

BOOST_SYMBOL_IMPORT boost::winapi::DWORD_ WINAPI GetThreadId(
	boost::winapi::HANDLE_ Thread);

#endif // !defined BOOST_USE_WINDOWS_H

namespace boost  {
namespace winapi {

	using ::GetThreadId;

} // end namespace winapi
} // end namespace boost