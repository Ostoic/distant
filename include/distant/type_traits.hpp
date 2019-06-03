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

	// Metafunction for detecting whether a type is a agents::kernel_object
	template <class T>
	struct is_kernel_object
	{
		using result =
			std::conditional_t<
				std::is_convertible<T, agents::kernel_object>::value, // If T derives from agents::kernel_object,
					std::true_type,					  // return true.
					std::false_type					  // Otherwise return false
			>;

		static constexpr bool value = result::value;
	};

	template <class T>
	struct sizeof_asserted
	{
		static_assert(!std::is_function_v<T>, "sizeof cannot take a function as an argument");
		static constexpr auto value = sizeof(T);
	};

	template <class T>
	constexpr auto sizeof_asserted_v = sizeof_asserted<T>::value;

	template <typename Handle>
	struct handle_traits;

	/// Contains kernel_object traits
	template <typename Agent>
	struct kernel_object_traits;

	struct default_kernel_object_traits
	{
		using handle_t = kernel_handle;
		using native_handle_t = boost::winapi::HANDLE_;
	};

} // end namespace distant