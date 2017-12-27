#pragma once

#include <boost\winapi\basic_types.hpp>
#include <distant\support\winapi\config.hpp>

BOOST_SYMBOL_IMPORT boost::winapi::BOOL_ WINAPI K32GetProcessMemoryInfo(
	boost::winapi::HANDLE_ Process,
	::PPROCESS_MEMORY_COUNTERS ppsmemCounters,
	boost::winapi::DWORD_ cb);

namespace boost::winapi
{
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
#endif

	BOOST_FORCEINLINE bool get_process_memory_info(
		boost::winapi::HANDLE_ Process,
		boost::winapi::PPROCESS_MEMORY_COUNTERS_ ppsmemCounters,
		boost::winapi::DWORD_ cb)
	{
		return ::K32GetProcessMemoryInfo(
			Process,
			reinterpret_cast<::PPROCESS_MEMORY_COUNTERS>(ppsmemCounters),
			cb);
	}

}

