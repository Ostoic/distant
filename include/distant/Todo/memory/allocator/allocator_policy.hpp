#pragma once

#include <cstddef>

#include <distant\memory\address.h>
#include <distant\process\process.h>

namespace distant {
	
	template <typename T>
	class allocation_policy
	{
	public:
		using value_type = T;
		using pointer = memory::address_type;
		using const_pointer = const memory::address_type;
		using reference = pointer&; // pointer& ?
		using const_reference = const pointer&;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;

	public:
		template <typename U>
		struct rebind {
			using other = allocation_policy<U>;
		};

	public:
		explicit allocation_policy() : m_process(distant::process::get_current()) {}
		~allocation_policy() {}

	private:
		const process& m_process;
	};


namespace detail {

	template <typename T>
	class object_traits
	{
	public:
		template <typename U>
		struct rebind { using other = object_traits<U>; };

	public:
		explicit object_traits() {}
		~object_traits() {}

		//memory::address_type 
	};

} // end namespace detail

} // end namespace distant