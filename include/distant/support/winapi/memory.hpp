#pragma once

#include <boost/winapi/basic_types.hpp>

#if !defined (BOOST_USE_WINDOWS_H)

BOOST_SYMBOL_IMPORT boost::winapi::BOOL_ WINAPI ReadProcessMemory(
	boost::winapi::HANDLE_ hProcess,
	boost::winapi::LPCVOID_ lpBaseAddress,
	boost::winapi::LPVOID_ lpBuffer,
	boost::winapi::SIZE_T_ nSize,
	boost::winapi::SIZE_T_ * lpNumberOfBytesRead);

BOOST_SYMBOL_IMPORT boost::winapi::BOOL_ WINAPI WriteProcessMemory(
	boost::winapi::HANDLE_ hProcess,
	boost::winapi::LPCVOID_ lpBaseAddress,
	boost::winapi::LPCVOID_ lpBuffer,
	boost::winapi::SIZE_T_ nSize,
	boost::winapi::SIZE_T_ * lpNumberOfBytesWritten);


#endif // !defined BOOST_USE_WINDOWS_H

namespace boost {
namespace winapi {

	using ::ReadProcessMemory;
	using ::WriteProcessMemory;

} // end namespace winapi
} // end namespace boost

