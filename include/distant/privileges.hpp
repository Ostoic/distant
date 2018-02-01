#pragma once

#include <distant\security\privileges.hpp>

namespace distant
{
	namespace privileges = distant::security::privileges;
	bool request_debug_privileges() noexcept;
}

// Implementation:
#include <distant\impl\privileges.hxx>