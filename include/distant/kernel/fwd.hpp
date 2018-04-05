#pragma once

#include <distant/access_rights.hpp>

namespace distant::kernel
{
	class kernel_object;
	class process_base;

	template <process_rights R>
	class process;

	template <token_rights R, typename Object>
	class access_token;
}
