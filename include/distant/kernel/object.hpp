#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <type_traits>

#include <distant\error\windows_error.hpp>

#include <distant\handle.hpp>
#include <distant\detail\handle_service.hpp>

#include <distant\utility\boolean_validator.hpp>

namespace distant::kernel {

	/// Base class for kernel objects
	class object : public utility::boolean_validator<object>
	{
	public:
		using error_type = distant::windows_error;
		using handle_type = distant::handle<object>;

	public:
		/// Bivariant type cast for kernel objects
		template <typename KernelObject,
			typename = std::enable_if_t<std::is_convertible<KernelObject, object>::value>>
		const handle<KernelObject>& get_handle() const noexcept;

		const handle<object>& get_handle() const noexcept;
		
		/// Invalid handle default constructor
		object() noexcept;

		/// Bivariant handle move constructor
		template <typename other_t>	
		explicit object(handle<other_t>&& h) noexcept;

		/// Move constructible
		object(object&& other) noexcept = default;

		/// Move assignable
		object& operator =(object&& other) noexcept = default;

		/// Check if the object handle is valid
		virtual bool valid() const noexcept;

		virtual ~object() = default;

	protected:
		handle_type m_handle;
		mutable error::windows_error m_last_error;
	};

} // end namespace distant::kernel

#include <distant\kernel\detail\object.hxx>

