#pragma once

#include <boost\winapi\basic_types.hpp>

namespace boost::winapi 
{
#if !defined (BOOST_USE_WINDOWS_H)

	constexpr boost::winapi::DWORD_ ANYSIZE_ARRAY_ = 1;

	typedef struct _LUID_ {
		boost::winapi::DWORD_ LowPart;
		boost::winapi::LONG_ HighPart;
	} LUID_, *PLUID_;

	typedef struct _LUID_AND_ATTRIBUTES_ {
		LUID_ Luid;
		boost::winapi::DWORD_ Attributes;
	} LUID_AND_ATTRIBUTES_, *PLUID_AND_ATTRIBUTES_;
	typedef LUID_AND_ATTRIBUTES_ LUID_AND_ATTRIBUTES_ARRAY_[ANYSIZE_ARRAY_];
	typedef LUID_AND_ATTRIBUTES_ARRAY_ *PLUID_AND_ATTRIBUTES_ARRAY_;

#else
	constexpr DWORD_ ANYSIZE_ARRAY_ = ANYSIZE_ARRAY;

	using _LUID_ = ::_LUID;
	using LUID_  = ::LUID;
	using PLUID_ = ::PLUID;

	using _LUID_AND_ATTRIBUTES_		  = ::_LUID_AND_ATTRIBUTES;
	using LUID_AND_ATTRIBUTES_		  = ::LUID_AND_ATTRIBUTES;
	using LUID_AND_ATTRIBUTES_ARRAY_  = ::LUID_AND_ATTRIBUTES_ARRAY;
	using PLUID_AND_ATTRIBUTES_ARRAY_ = ::PLUID_AND_ATTRIBUTES_ARRAY;

	using PRIVILEGE_SET_ = ::PRIVILEGE_SET;
	using PPRIVILEGE_SET_ = ::PPRIVILEGE_SET;

#endif // !defined BOOST_USE_WINDOWS_H

} // end namespace winapi
