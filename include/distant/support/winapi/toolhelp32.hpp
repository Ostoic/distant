#pragma once

#include <boost\winapi\limits.hpp>

#ifndef DISTANT_TLHLP32_INCLUDED
#include <Tlhelp32.h>
#define DISTANT_TLHLP32_INCLUDED
#endif

BOOST_SYMBOL_IMPORT boost::winapi::HANDLE_ WINAPI CreateToolhelp32Snapshot(
	boost::winapi::DWORD_ dwFlags,
	boost::winapi::DWORD_ th32ProcessID);

BOOST_SYMBOL_IMPORT boost::winapi::BOOL_ WINAPI Process32First(
	boost::winapi::HANDLE_ hSnapshot,
	::LPPROCESSENTRY32 lppe);

BOOST_SYMBOL_IMPORT boost::winapi::BOOL_ WINAPI Process32Next(
	boost::winapi::HANDLE_ hSnapshot,
	::LPPROCESSENTRY32 lppe);

namespace boost::winapi
{
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

	BOOST_FORCEINLINE boost::winapi::BOOL_ process32_next(
		boost::winapi::HANDLE_ hSnapshot,
		boost::winapi::LPPROCESSENTRY32_ lppe)
	{
		return ::Process32Next(hSnapshot, lppe);
	}

	BOOST_FORCEINLINE boost::winapi::BOOL_ process32_first(
		boost::winapi::HANDLE_ hSnapshot,
		boost::winapi::LPPROCESSENTRY32_ lppe)
	{
		return ::Process32First(hSnapshot, lppe);
	}
}
