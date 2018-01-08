#pragma once

#include <boost\winapi\basic_types.hpp>

#if !defined (BOOST_USE_WINDOWS_H)

#if !defined (BOOST_NO_ANSI_APIS)
BOOST_SYMBOL_IMPORT boost::winapi::UINT_ WINAPI GetWindowsDirectoryA(
	boost::winapi::LPSTR_ lpBuffer,
	boost::winapi::UINT_ uSize);

#endif // !defined no ansi apis

BOOST_SYMBOL_IMPORT boost::winapi::UINT_ WINAPI GetWindowsDirectoryW(
	boost::winapi::LPWSTR_ lpBuffer,
	boost::winapi::UINT_ uSize);


#if BOOST_USE_WINAPI_VERSION >= BOOST_WINAPI_VERSION_WIN8
struct FIRMWARE_TYPE;

BOOST_SYMBOL_IMPORT boost::winapi::BOOL_ WINAPI GetFirmwareType(
	::PFIRMWARE_TYPE FirmwareType);

BOOST_SYMBOL_IMPORT boost::winapi::BOOL_ WINAPI IsNativeVhdBoot(
	boost::winapi::PBOOL_ NativeVhdBoot);

#if !defined (BOOST_NO_ANSI_APIS)
BOOST_SYMBOL_IMPORT boost::winapi::BOOL_ WINAPI GetComputerNameA(
	boost::winapi::LPSTR_ lpBuffer,
	boost::winapi::LPDWORD_ nSize);
#endif // !defined no ansi apis

BOOST_SYMBOL_IMPORT boost::winapi::BOOL_ WINAPI GetComputerNameW(
	boost::winapi::LPWSTR_ lpBuffer,
	boost::winapi::LPDWORD_ nSize);

BOOST_SYMBOL_IMPORT boost::winapi::BOOL_ WINAPI GetUserNameA(
	boost::winapi::LPSTR_ lpBuffer,
	boost::winapi::LPDWORD_ pcbBuffer);

BOOST_SYMBOL_IMPORT boost::winapi::BOOL_ WINAPI GetUserNameW(
	boost::winapi::LPWSTR_ lpBuffer,
	boost::winapi::LPDWORD_ pcbBuffer);

#endif // win8 version check

#endif // !defined BOOST_USE_WINDOWS_H

namespace boost::winapi {

#if !defined (BOOST_NO_ANSI_APIS)
	using ::GetWindowsDirectoryA;
	using ::GetComputerNameA;
	using ::GetUserNameA;
#endif
	using ::GetWindowsDirectoryW;
	using ::GetComputerNameW;
	using ::GetUserNameW;

	using ::IsNativeVhdBoot;

	constexpr DWORD_ UNLEN_ = 256;

#if !defined (BOOST_USE_WINDOWS_H)
	constexpr DWORD_ MAX_COMPUTERNAME_LENGTH_ = 15;
#else
	constexpr DWORD_ MAX_COMPUTERNAME_LENGTH_ = MAX_COMPUTERNAME_LENGTH;
#endif

#if BOOST_USE_WINAPI_VERSION >= BOOST_WINAPI_VERSION_WIN8
	typedef enum _FIRMWARE_TYPE_ {
		FirmwareTypeUnknown,
		FirmwareTypeBios,
		FirmwareTypeUefi,
		FirmwareTypeMax
	} FIRMWARE_TYPE_, *PFIRMWARE_TYPE_;

	BOOST_FORCEINLINE boost::winapi::BOOL_ get_firmware_type(boost::winapi::FIRMWARE_TYPE_* FirmwareType) noexcept
	{
		return ::GetFirmwareType(reinterpret_cast<FIRMWARE_TYPE*>(FirmwareType));
	}
#endif
}
