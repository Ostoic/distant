#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

// For Windows SDK selection
#include <ntverp.h>

#include <distant\detail\attorney.hpp>

#include <distant\access_rights.hpp>
#include <distant\detail\handle_base.hpp>

namespace distant {

	/// Type-safe handle for (kernel) objects
	template <typename Object_t> 
	class handle : public detail::handle_base 
	{
	private:
		using handle_base = detail::handle_base;

	public:
		using object_type = Object_t;

	public:
		/// \param h the native handle value
		/// \param flags handle flags 
		constexpr explicit handle(native_type h, flag_type flags = flag_type::inherit) noexcept;

		/// Invalid handle literal constructor
		constexpr handle() noexcept;

		/// Invalid handle literal constructor
		constexpr handle(detail::invalid_t) noexcept;

		/// Bivariant move constructor
		template <typename other_t>
		handle(handle<other_t>&& other) noexcept;
		
		/// Bivariant move assignment
		template <typename other_t>
		handle& operator =(handle<other_t>&& other) noexcept;
		
	private:	
		/// Allow attorney to expose some implementation details
		// This is mainly for recreated winapi functions to pass
		// the underlying handle value into the winapi.
		template <typename>
		friend class distant::detail::attorney::to_handle;

		// Expose implementation to other handle types
		template <typename>
		friend class handle;

	public:
		template <typename T, typename U>
		friend constexpr bool operator ==(const handle<T>&, const handle<U>&) noexcept;

		template <typename T, typename U>
		friend constexpr bool operator !=(const handle<T>&, const handle<U>&) noexcept;
	};

} // end namespace distant

#include <distant\detail\handle.hxx>
