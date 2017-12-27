#pragma once

#include <boost\winapi\basic_types.hpp>

namespace distant::winapi 
{
	typedef struct _LUID_ {
		boost::winapi::DWORD_ LowPart;
		boost::winapi::LONG_ HighPart;
	} LUID_, *PLUID_;

	typedef struct _LUID_AND_ATTRIBUTES_ {
		LUID_ Luid;
		boost::winapi::DWORD_ Attributes;
	} LUID_AND_ATTRIBUTES_, *PLUID_AND_ATTRIBUTES_;
	typedef LUID_AND_ATTRIBUTES_ LUID_AND_ATTRIBUTES_ARRAY_[ANYSIZE_ARRAY];
	typedef LUID_AND_ATTRIBUTES_ARRAY_ *PLUID_AND_ATTRIBUTES_ARRAY_;

} // end namespace winapi
