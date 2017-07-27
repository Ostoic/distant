#pragma once

#include <type_traits>

#include <distant\detail\fwd.hpp>

namespace distant {
namespace windows {

	template <class T>
	struct is_kernel_object : std::false_type {};

	template <>
	struct is_kernel_object<kernel::object> : std::true_type {};

	template <>
	struct is_kernel_object<kernel::securable> : std::true_type {};

	template <access_rights::process T>
	struct is_kernel_object<kernel::process<T>> : std::true_type {};

	template <class T>
	struct is_interoperable
	{

	};

	// Traits
	template <class T>
	struct object_traits 
	{
		static_assert (
			is_kernel_object<T>::value,
			"Type must derive from windows kernel object");

		using handle_type = windows::handle<T>;
		using error_type = windows::error::gle;
	};

	//template <>
	//struct object_traits<tag::self>
	//{
	//	//using handle_type = typename Object_t::handle_type;
	//	//using error_type = typename Object_t::error_type;
	//};

} // end namespace windows

using windows::object_traits;

} // end namespace distant