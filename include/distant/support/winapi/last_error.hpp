/*
* Copyright 2017 Shaun Ostoic
*
* Distributed under the Boost Software License, Version 1.0.
* See http://www.boost.org/LICENSE_1_0.txt
*/

#pragma once

#include <boost\winapi\basic_types.hpp>
#include <distant\support\winapi\config.hpp>

BOOST_SYMBOL_IMPORT boost::winapi::VOID_ WINAPI SetLastError(
	boost::winapi::DWORD_ error_code);

namespace boost {
	namespace winapi {
		using ::SetLastError;
	}
}
