#pragma once

namespace distant::system
{
	enum class processor_architecture
	{
		amd64 = 9,
		ia64 = 6,
		arm = 5,
		intel = 0,
		unknown = 0xffff,
	};
}
