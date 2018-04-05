#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <type_traits>

#include <distant/detail/fwd.hpp>

namespace distant {

	template <class A, class B>
	using is_quasiconvertible = 
		std::conditional_t<
			std::is_convertible<A, B>::value ||	// If A is convertible to B,
			std::is_convertible<B, A>::value,	// or if B is convertible to A,
				std::true_type,				// then return true.
				std::false_type				// Otherwise return false
		>;

	// Metafunction for detecting whether a type is a kernel::kernel_object
	template <class T>
	struct is_kernel_object
	{
		using result = typename
			std::conditional_t<
				std::is_convertible<T, kernel::kernel_object>::value, // If T derives from kernel::kernel_object,
					std::true_type,					  // return true.
					std::false_type					  // Otherwise return false
			>;

		static constexpr bool value = result::value;
	};

	/// Contains kernel kernel_object traits
	template <class T>
	struct kernel_object_traits 
	{
		using handle_type = handle<T>;
		using error_type  = error::windows_error_code;
		using object_type = T;
	};

	template <typename Object>
	struct get_access_rights;

} // end namespace distant