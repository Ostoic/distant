#pragma once

#include <WinBase.h>

namespace distant::windows::kernel {

	enum class priority_class 
	{
		idle = IDLE_PRIORITY_CLASS,
	};

} // end namespace distant::windows::kernel