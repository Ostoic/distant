#pragma once

#include <type_traits>

namespace distant::utility::meta
{
	template <class T>
	using remove_cvref = std::remove_reference_t<std::remove_cv_t<T>>;
}
