// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <boost/winapi/basic_types.hpp>

#if !defined (BOOST_USE_WINDOWS_H)

struct PROCESS_MEMORY_COUNTERS;
struct PERFORMANCE_INFORMATION;

BOOST_SYMBOL_IMPORT boost::winapi::BOOL_ WINAPI K32GetProcessMemoryInfo(
	boost::winapi::HANDLE_ Process,
	::PROCESS_MEMORY_COUNTERS* ppsmemCounters,
	boost::winapi::DWORD_ cb
);

BOOST_SYMBOL_IMPORT boost::winapi::BOOL_ WINAPI GetPerformanceInfo(
	_Out_ ::PERFORMANCE_INFORMATION* pPerformanceInformation,
	_In_  DWORD                    cb
);

#endif // !defined BOOST_USE_WINDOWS_H

namespace boost  {
namespace winapi {

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

	typedef struct _PERFORMANCE_INFORMATION_ {
		DWORD_  cb;
		SIZE_T_ CommitTotal;
		SIZE_T_ CommitLimit;
		SIZE_T_ CommitPeak;
		SIZE_T_ PhysicalTotal;
		SIZE_T_ PhysicalAvailable;
		SIZE_T_ SystemCache;
		SIZE_T_ KernelTotal;
		SIZE_T_ KernelPaged;
		SIZE_T_ KernelNonpaged;
		SIZE_T_ PageSize;
		DWORD_  HandleCount;
		DWORD_  ProcessCount;
		DWORD_  ThreadCount;
	} PERFORMANCE_INFORMATION_, *PPERFORMANCE_INFORMATION_;

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

	BOOST_FORCEINLINE bool get_process_memory_info(
		boost::winapi::HANDLE_ Process,
		boost::winapi::PPROCESS_MEMORY_COUNTERS_ ppsmemCounters,
		boost::winapi::DWORD_ cb)
	{
		return ::K32GetProcessMemoryInfo(
			Process,
			reinterpret_cast<::PROCESS_MEMORY_COUNTERS*>(ppsmemCounters),
			cb
		);
	}

	BOOST_FORCEINLINE bool get_performance_info(
		PERFORMANCE_INFORMATION_* performance_information,
		DWORD_ cb)
	{
		return ::GetPerformanceInfo(
			reinterpret_cast<::PERFORMANCE_INFORMATION*>(performance_information),
			cb
		);
	}

} // namespace winapi
} // namespace boost
