#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <type_traits>

#include <distant\detail\fwd.hpp>

namespace distant {
namespace utility {

	namespace detail { class force_relation {}; }

	template <class A, class B>
	using is_related = 
		std::conditional_t<
			std::is_base_of<A, B>::value ||	// If A is a base of B,
			std::is_base_of<B, A>::value ||	// or B is a base of A,
			std::is_same<A, detail::force_relation>::value || // or if the user is forcing a relation on the two types
			std::is_same<B, detail::force_relation>::value,  
				std::true_type,				// then return true.
				std::false_type				// Otherwise return false
		>;

} // end namespace utility

	// Metafunction for detecting whether a type is a kernel::object
	template <class T>
	struct is_kernel_object
	{
		using result = typename
			std::conditional_t<
				std::is_base_of_v<kernel::object, T>, // If T derives from kernel::object,
					std::true_type,					  // return true.
					std::false_type					  // Otherwise return false
			>;

		static constexpr bool value = result::value;
	};

	template <>
	struct is_kernel_object<kernel::object> : std::true_type {};

	template <>
	struct is_kernel_object<kernel::securable> : std::true_type {};

	//template <>
	//struct is_kernel_object<kernel::detail::process_base> : std::true_type {};

	template <access_rights::process T>
	struct is_kernel_object<kernel::process<T>> : std::true_type {};

	/// Contains kernel object traits
	template <class T>
	struct object_traits 
	{
		/*static_assert (
			is_kernel_object<T>::value,
			"Type must derive from kernel::object");*/

		using handle_type = handle<T>;
		using error_type  = error::gle;
		using object_type = T;
	};

} // end namespace distant