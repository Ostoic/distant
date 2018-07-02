// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <type_traits>

#include <distant/fwd.hpp>

namespace distant {

	template <class A, class B>
	using is_quasiconvertible = 
		std::conditional_t<
			std::is_convertible<A, B>::value ||	// If A is convertible to B,
			std::is_convertible<B, A>::value,	// or if B is convertible to A,
				std::true_type,				// then return true.
				std::false_type				// Otherwise return false
		>;

	// Metafunction for detecting whether a type is a kernel_objects::kernel_object
	template <class T>
	struct is_kernel_object
	{
		using result = 
			std::conditional_t<
				std::is_convertible<T, kernel_objects::kernel_object>::value, // If T derives from kernel_objects::kernel_object,
					std::true_type,					  // return true.
					std::false_type					  // Otherwise return false
			>;

		static constexpr bool value = result::value;
	};

	/// Contains kernel_object traits
	template <typename KernelObject>
	struct kernel_object_traits;

	template <typename KernelObject>
	struct default_kernel_object_traits
	{
		using handle_type = distant::unsafe_handle;
		using native_handle_type = boost::winapi::HANDLE_;
	};

	template <typename Object>
	struct get_access_rights;

} // end namespace distant