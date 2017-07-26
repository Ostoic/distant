#pragma once

#include <type_traits>

#include <distant\detail\fwd.hpp>

namespace distant {
namespace windows {

template <typename>
class is_object : public std::false_type {};
	
template <>
class is_object<windows::kernel::object> : public std::true_type {};

} // end namespace windows
} // end namespace distant