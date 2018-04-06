// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <boost/winapi/limits.hpp>

#if !defined (BOOST_USE_WINDOWS_H)

struct PROCESSENTRY32;
struct PROCESSENTRY32W;

BOOST_SYMBOL_IMPORT boost::winapi::BOOL_ WINAPI Process32FirstW(
	boost::winapi::HANDLE_ hSnapshot,
	::PROCESSENTRY32W* lppe
);

BOOST_SYMBOL_IMPORT boost::winapi::BOOL_ WINAPI Process32NextW(
	boost::winapi::HANDLE_ hSnapshot,
	::PROCESSENTRY32W* lppe
);

BOOST_SYMBOL_IMPORT boost::winapi::HANDLE_ WINAPI CreateToolhelp32Snapshot(
	boost::winapi::DWORD_ dwFlags,
	boost::winapi::DWORD_ th32ProcessID);

BOOST_SYMBOL_IMPORT boost::winapi::BOOL_ WINAPI Process32First(
	boost::winapi::HANDLE_ hSnapshot,
	::PROCESSENTRY32* lppe);

BOOST_SYMBOL_IMPORT boost::winapi::BOOL_ WINAPI Process32Next(
	boost::winapi::HANDLE_ hSnapshot,
	::PROCESSENTRY32* lppe);

#endif

namespace boost  {
namespace winapi {

#if !defined (BOOST_USE_WINDOWS_H)
	typedef struct tagPROCESSENTRY32W_
	{
		DWORD_   dwSize;
		DWORD_   cntUsage;
		DWORD_   th32ProcessID;          // this process
		ULONG_PTR_ th32DefaultHeapID;
		DWORD_   th32ModuleID;           // associated exe
		DWORD_   cntThreads;
		DWORD_   th32ParentProcessID;    // this process's parent process
		LONG_    pcPriClassBase;         // Base priority of process's threads
		DWORD_   dwFlags;
		WCHAR_   szExeFile[MAX_PATH_];    // Path
	} PROCESSENTRY32W_;
	typedef PROCESSENTRY32W_ *  PPROCESSENTRY32W_;
	typedef PROCESSENTRY32W_ *  LPPROCESSENTRY32W_;

	typedef struct tagPROCESSENTRY32_
	{
		DWORD_   dwSize;
		DWORD_   cntUsage;
		DWORD_   th32ProcessID;          // this process
		ULONG_PTR_ th32DefaultHeapID;
		DWORD_   th32ModuleID;           // associated exe
		DWORD_   cntThreads;
		DWORD_   th32ParentProcessID;    // this process's parent process
		LONG_    pcPriClassBase;         // Base priority of process's threads
		DWORD_   dwFlags;
		CHAR_    szExeFile[boost::winapi::max_path];    // Path
	} PROCESSENTRY32_;
	typedef PROCESSENTRY32_ *  PPROCESSENTRY32_;
	typedef PROCESSENTRY32_ *  LPPROCESSENTRY32_;

	constexpr DWORD_ TH32CS_SNAPHEAPLIST_ = 0x00000001;
	constexpr DWORD_ TH32CS_SNAPPROCESS_ = 0x00000002;
	constexpr DWORD_ TH32CS_SNAPTHREAD_ = 0x00000004;
	constexpr DWORD_ TH32CS_SNAPMODULE_ = 0x00000008;
	constexpr DWORD_ TH32CS_SNAPMODULE32_ = 0x00000010;
	constexpr DWORD_ TH32CS_SNAPALL_ = (TH32CS_SNAPHEAPLIST_ | TH32CS_SNAPPROCESS_ | TH32CS_SNAPTHREAD_ | TH32CS_SNAPMODULE_);
	constexpr DWORD_ TH32CS_INHERIT_ = 0x80000000;

#else
	using tagPROCESSENTRY32W_ = ::tagPROCESSENTRY32W;
	using PROCESSENTRY32W_ = ::PROCESSENTRY32W;
	using PPROCESSENTRY32W_ = ::PPROCESSENTRY32W;
	using LPPROCESSENTRY32W_ = ::LPPROCESSENTRY32W;

	using tagPROCESSENTRY32_ = ::tagPROCESSENTRY32;
	using PROCESSENTRY32_ = ::PROCESSENTRY32;
	using PPROCESSENTRY32_ = ::PPROCESSENTRY32;
	using LPPROCESSENTRY32_ = ::LPPROCESSENTRY32;

	constexpr DWORD_ TH32CS_SNAPHEAPLIST_ = TH32CS_SNAPHEAPLIST;
	constexpr DWORD_ TH32CS_SNAPPROCESS_ = TH32CS_SNAPPROCESS;
	constexpr DWORD_ TH32CS_SNAPTHREAD_ = TH32CS_SNAPTHREAD;
	constexpr DWORD_ TH32CS_SNAPMODULE_ = TH32CS_SNAPMODULE;
	constexpr DWORD_ TH32CS_SNAPMODULE32_ = TH32CS_SNAPMODULE32;
	constexpr DWORD_ TH32CS_SNAPALL_ = TH32CS_SNAPALL;
	constexpr DWORD_ TH32CS_INHERIT_ = TH32CS_INHERIT;
#endif

	using ::CreateToolhelp32Snapshot;

	BOOST_FORCEINLINE boost::winapi::BOOL_ process32_next(
		boost::winapi::HANDLE_ hSnapshot,
		boost::winapi::PROCESSENTRY32_* lppe)
	{
		return ::Process32Next(hSnapshot, reinterpret_cast<::PROCESSENTRY32*>(lppe));
	}

	BOOST_FORCEINLINE boost::winapi::BOOL_ process32_first(
		boost::winapi::HANDLE_ hSnapshot,
		boost::winapi::PROCESSENTRY32_* lppe)
	{
		return ::Process32First(hSnapshot, reinterpret_cast<::PROCESSENTRY32*>(lppe));
	}

} // namespace winapi
} // namespace boost