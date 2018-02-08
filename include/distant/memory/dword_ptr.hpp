#pragma once

#include <distant\memory\address.hpp>
#include <distant\memory\x86_register.hpp>

namespace distant::memory
{
	template <typename T = 
		std::enable_if_t<
			std::is_same_v<x86_register, T> || std::is_same_v<distant::address, T>, 
			T
		>
	>
	struct dword_ptr_t 
	{
		explicit constexpr dword_ptr_t(T reg_address) noexcept;

		T data_;
	};
}

#include <distant\memory\impl\dword_ptr.hxx>