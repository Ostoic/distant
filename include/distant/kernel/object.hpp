#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <type_traits>

#include <distant\error\windows_error.hpp>

#include <distant\handle.hpp>

#include <distant\utility\boolean_validator.hpp>

namespace distant::kernel {

	/// Base class for kernel objects
	class object : public utility::boolean_validator<object>
	{
	public:
		using error_type = distant::windows_error;
		using handle_type = distant::handle<object>;
		 
	public: // interface
		/// Bivariant type cast for kernel objects
		template <typename KernelObject,
			typename = std::enable_if_t<std::is_convertible<KernelObject, object>::value>>
		const handle<KernelObject>& get_handle() const noexcept;

		/// Get a handle to the object.
		/// \return a type-safe handle to the object.
		const handle<object>& get_handle() const noexcept;

		/// Check if the object is valid
		/// \return true if the object is valid.
		virtual bool valid() const noexcept;

		/// Declare the destructor virtual to prevent slicing.
		virtual ~object() = default;

	public: // {ctor}
		/// Invalid handle default constructor
		object() noexcept;

		explicit object(handle_type&& handle) noexcept;

		/// Move constructible
		object(object&& other) noexcept = default;

		/// Move assignable
		object& operator =(object&& other) noexcept = default;

	protected:
		handle_type m_handle;
		mutable windows_error m_error;
	};

} // end namespace distant::kernel

#include <distant\kernel\impl\object.hxx>

