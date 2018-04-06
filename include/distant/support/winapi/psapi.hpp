// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <boost/winapi/basic_types.hpp>

#if !defined (BOOST_USE_WINDOWS_H)

struct PROCESS_MEMORY_COUNTERS;

BOOST_SYMBOL_IMPORT boost::winapi::BOOL_ WINAPI K32GetProcessMemoryInfo(
	boost::winapi::HANDLE_ Process,
	::PROCESS_MEMORY_COUNTERS* ppsmemCounters,
	boost::winapi::DWORD_ cb);
#endif // !defined BOOST_USE_WINDOWS_H

namespace boost  {
namespace winapi {

#if !defined (BOOST_USE_WINDOWS_H)

	typedef struct _PROCESS_MEMORY_COUNTERS_ {
		boost::winapi::DWORD_ cb;
		boost::winapi::DWORD_ PageFaultCount;
		boost::winapi::SIZE_T_ PeakWorkingSetSize;
		boost::winapi::SIZE_T_ WorkingSetSize;
		boost::winapi::SIZE_T_ QuotaPeakPagedPoolUsage;
		boost::winapi::SIZE_T_ QuotaPagedPoolUsage;
		boost::winapi::SIZE_T_ QuotaPeakNonPagedPoolUsage;
		boost::winapi::SIZE_T_ QuotaNonPagedPoolUsage;
		boost::winapi::SIZE_T_ PagefileUsage;
		boost::winapi::SIZE_T_ PeakPagefileUsage;
	} PROCESS_MEMORY_COUNTERS_;
	typedef PROCESS_MEMORY_COUNTERS_ *PPROCESS_MEMORY_COUNTERS_;

#if BOOST_USE_WINAPI_VERSION >= BOOST_WINAPI_VERSION_WINXP
	typedef struct _PROCESS_MEMORY_COUNTERS_EX_ {
		boost::winapi::DWORD_ cb;
		boost::winapi::DWORD_ PageFaultCount;
		boost::winapi::SIZE_T_ PeakWorkingSetSize;
		boost::winapi::SIZE_T_ WorkingSetSize;
		boost::winapi::SIZE_T_ QuotaPeakPagedPoolUsage;
		boost::winapi::SIZE_T_ QuotaPagedPoolUsage;
		boost::winapi::SIZE_T_ QuotaPeakNonPagedPoolUsage;
		boost::winapi::SIZE_T_ QuotaNonPagedPoolUsage;
		boost::winapi::SIZE_T_ PagefileUsage;
		boost::winapi::SIZE_T_ PeakPagefileUsage;
		boost::winapi::SIZE_T_ PrivateUsage;
	} PROCESS_MEMORY_COUNTERS_EX_;
	typedef PROCESS_MEMORY_COUNTERS_EX_ *PPROCESS_MEMORY_COUNTERS_EX_;
#endif // >= winxpi version check

#endif // !defined BOOST_USE_WINDOWS_H

	BOOST_FORCEINLINE bool get_process_memory_info(
		boost::winapi::HANDLE_ Process,
		boost::winapi::PPROCESS_MEMORY_COUNTERS_ ppsmemCounters,
		boost::winapi::DWORD_ cb)
	{
		return ::K32GetProcessMemoryInfo(
			Process,
			reinterpret_cast<::PROCESS_MEMORY_COUNTERS*>(ppsmemCounters),
			cb);
	}

} // namespace winapi
} // namespace boost
