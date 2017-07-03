#pragma once

#include <Windows.h>
#include <cstddef>

#include <distant\process\process.h>

namespace distant {
	
	template <typename T>
	class allocation_policy
	{
	public:
		using value_type = T;
		using pointer = DWORD;
		using const_pointer = const DWORD;
		using reference = value_type&; // pointer& ?
		using const_reference = const value_type&;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;

	public:
		template <typename U>
		struct rebind {
			using other = allocation_policy<U>;
		};

	public:
		inline explicit allocation_policy() : m_process(distant::process::get_current()) {}
		inline ~allocation_policy() { }

	private:
		distant::process m_process;
	};

} // end namespace distant