// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/type_traits.hpp>
#include <distant/unsafe_handle.hpp>

namespace distant
{		
	// Todo: Closure policies for different types of handles CloseHandle, 
	/// Type-safe handle for windows objects
	template <typename ObjectT>
	class handle : public unsafe_handle
	{
	private:
		using handle_base = unsafe_handle;

	public:
		using object_type = ObjectT;

	public: // {ctor}
		/// @param native_handle the native handle value.
		/// @param flags handle flags .
		/// @param closed whether or not the handle is closed.
		explicit constexpr handle(native_type native_handle, flag_type flags = flag_type::inherit, bool closed = false) noexcept;

		/// Construct an invalid handle.
		/// This allows handles to be comparable with nullptr.
		/// @param h the nullptr.
		constexpr handle(nullptr_t h) noexcept;

		/// Invalid handle literal constructor
		constexpr handle() noexcept;

		template <typename OtherT, typename = std::enable_if_t<is_quasiconvertible<ObjectT, OtherT>::value>>
		handle(handle<OtherT>&& other) noexcept;

		template <typename OtherT, typename = std::enable_if_t<is_quasiconvertible<ObjectT, OtherT>::value>>
		handle& operator=(handle<OtherT>&&) noexcept;

		handle(handle&&) noexcept = default;
		handle& operator=(handle&&) noexcept = default;

		template <typename U>
		void swap(handle<U>& other) noexcept
		{
			std::swap(native_handle_, other.native_handle_);
			std::swap(flags_, other.flags_);
		}

	private:
		// Expose implementation to other handle types
		template <typename>
		friend class handle;

	public:
		template <typename T, typename U>
		friend constexpr bool operator ==(const handle<T>&, const handle<U>&) noexcept;

		template <typename T, typename U>
		friend constexpr bool operator !=(const handle<T>&, const handle<U>&) noexcept;
	};

	template <typename T, typename U>
	void swap(distant::handle<T>& lhs, distant::handle<U>& rhs) noexcept
	{
		lhs.swap(rhs);
	}

} // end namespace distant


#include "impl/handle.hxx"
