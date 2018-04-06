// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once
/*
* Copyright 2017 Shaun Ostoic
*
* Distributed under the Boost Software License, Version 1.0.
* See http://www.boost.org/LICENSE_1_0.txt
*/


#include <boost/winapi/basic_types.hpp>

#if !defined (BOOST_USE_WINDOWS_H)

BOOST_SYMBOL_IMPORT boost::winapi::VOID_ WINAPI SetLastError(
	boost::winapi::DWORD_ error_code);

#endif // !defined BOOST_USE_WINDOWS_H

namespace boost {
	namespace winapi {
		using ::SetLastError;
	}
}
