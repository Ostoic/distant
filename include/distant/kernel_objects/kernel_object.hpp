// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <type_traits>

#include <distant/config.hpp>
#include <distant/error/winapi_error.hpp>

#include <distant/scoped_handle.hpp>

#include <distant/concepts/boolean_validator.hpp>
#include "fwd.hpp"

namespace distant::kernel_objects
{
	/// @brief Base class for kernel objects
	class kernel_object : public concepts::boolean_validator<kernel_object>
	{
	public: // interface
		/// @brief Get a handle to the kernel_object.
		/// @return a type-safe handle to the kernel_object.
		const kernel_handle& handle() const noexcept;

		/// @brief Check if the kernel_object is valid
		/// @return true if the kernel_object is valid.
		virtual bool valid() const noexcept;

		/// Declare the destructor virtual to prevent slicing.
		virtual ~kernel_object() = default;

	public: // {ctor}
		/// @brief Invalid handle default constructor
		kernel_object() noexcept;

		explicit kernel_object(distant::kernel_handle& handle) noexcept;

		/// @brief Move constructible
		kernel_object(kernel_object&& other) noexcept = default;

		/// @brief Move assignable
		kernel_object& operator=(kernel_object&& other) noexcept = default;

	protected:
		kernel_handle handle_;
	};

} // end namespace distant::kernel_objects

#include "impl/kernel_object.hxx"
