// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <type_traits>

#include <distant/config.hpp>
#include <distant/error/windows_error.hpp>

#include <distant/handle.hpp>

#include <distant/utility/boolean_validator.hpp>
#include "fwd.hpp"

namespace distant::kernel_objects
{
	/// @brief Base class for kernel objects
	class kernel_object : public utility::boolean_validator<kernel_object>
	{
	public:
		using handle_type = handle<kernel_object>;

	public: // interface
		/// @brief Bivariant type cast for kernel objects
		template <typename KernelObject,
		          typename = std::enable_if_t<std::is_convertible<KernelObject, kernel_object>::value>>
		const distant::handle<KernelObject>& handle() const noexcept;

		/// @brief Get a handle to the kernel_object.
		/// @return a type-safe handle to the kernel_object.
		const distant::handle<kernel_object>& handle() const noexcept;

		/// @brief Check if the kernel_object is valid
		/// @return true if the kernel_object is valid.
		virtual bool valid() const noexcept;

		/// Declare the destructor virtual to prevent slicing.
		virtual ~kernel_object() = default;

	public: // {ctor}
		/// @brief Invalid handle default constructor
		kernel_object() noexcept;

		explicit kernel_object(handle_type&& handle) noexcept;

		/// @brief Move constructible
		kernel_object(kernel_object&& other) noexcept = default;

		/// @brief Move assignable
		kernel_object& operator=(kernel_object&& other) noexcept = default;

	protected:
		handle_type handle_;
	};
} // end namespace distant::kernel_objects

#include <distant/kernel_objects/impl/kernel_object.hxx>
