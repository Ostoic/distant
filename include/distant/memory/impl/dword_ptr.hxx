#pragma once
#include <distant\memory\dword_ptr.hpp>

namespace distant::memory
{
//class dword_ptr
//public:
	template<typename T>
	constexpr dword_ptr_t<T>::dword_ptr_t(T data) noexcept
		: data_(data) {}
}
