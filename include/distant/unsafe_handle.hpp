// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/access_rights.hpp>
#include <distant/type_traits.hpp>
#include <distant/concepts/boolean_validator.hpp>

#include <boost/winapi/config.hpp>
#include <boost/winapi/basic_types.hpp>
#include <boost/winapi/handles.hpp>

#include <bitset>

namespace distant 
{
	template <typename HandleTraits>
	class unsafe_handle : public concepts::boolean_validator<unsafe_handle<HandleTraits>>
	{
	public:
		// Underlying handle type. This is macro'd in Windows to be void* == (HANDLE)
		using native_t = typename handle_traits<unsafe_handle>::native_t;
		using traits_t = HandleTraits;
		using flag_t = access_rights::handle;

	public:
		/// Construct using native handle.
		/// @param h the native handle value
		/// @param flags handle flags 
		explicit constexpr unsafe_handle(native_t h, flag_t flags = flag_t::inherit, bool closed = false) noexcept;

		/// Construct an invalid handle.
		/// This allows handles to be comparable with nullptr.
		/// @param h the nullptr.
		constexpr unsafe_handle(nullptr_t h) noexcept;

		/// Construct invalid handle.
		/// This calls the nullptr constructor.
		constexpr unsafe_handle() noexcept;

		/// Move copyable
		unsafe_handle(unsafe_handle&&) noexcept;

		/// Move assignable
		unsafe_handle& operator=(unsafe_handle&&) noexcept;

		// If we allow copy ctor/assignment, then multiple copies will eventually attempt 
		// to close the same handle, which is not desirable.
		unsafe_handle(const unsafe_handle&) = delete;
		unsafe_handle& operator =(const unsafe_handle&) = delete;

		/// Close handle to windows object.
		/// Handle must be weakly valid in order to close the handle.
		~unsafe_handle() noexcept { this->close(); }
		
	public:
		/// Checks the if the native handle is valid
		/// @return true if the native_handle is not NULL, and false otherwise
		bool valid() const noexcept;

		/// Check if the handle is close protected
		/// @return true if the handle cannot be closed, false otherwise
		bool close_protected() const noexcept;

		// Note: This function is public since handles occasionally need to be closed before the
		// stack unwind.
		/// Check if handle's closure has been observed
		/// @return true if the handle's closure was observed, and false otherwise
		bool closed() const noexcept;

		/// Close the handle, if it is valid and its closure wasn't observed
		bool close() noexcept;

		/// Get the value of the native handle
		/// @return value of the native handle
		native_t native_handle() const noexcept;

		template <typename OtherClose>
		constexpr bool equals(const unsafe_handle<OtherClose>& other) const noexcept;

	protected:
		// From "Windows Via C\C++" by Jeffrey Richter,
		// setting the handle to null is preferable to invalid_handle
		// after closing the handle. This is probably because some API
		// calls consider invalid_handle as the current process/thread.
		/// Numerically invalidate and close-protect our handle.
		void invalidate() noexcept;

		/// Protect the handle from being closed
		void protect() noexcept;

		/// Get the handle's flag type
		/// @return distant::access_rights::handle flag type
		flag_t flags() const noexcept;

	protected:
		/// native HANDLE value
		native_t native_handle_;

		// If we somehow attempt to call CloseHandle multiple times,
		// this will help prevent further unnecessary calls.
		/// Switch to check if closure was observed 
		std::bitset<3> flags_;
		
		template <typename OtherClose>
		friend class unsafe_handle;
	};

	struct kernel_handle_traits
	{
		using native_t = boost::winapi::HANDLE_;

		static bool close(const native_t native_handle) noexcept
		{
			return boost::winapi::CloseHandle(native_handle);
		}
	};

	template <typename Traits>
	struct handle_traits<unsafe_handle<Traits>>
		: Traits {};

	using kernel_handle = unsafe_handle<kernel_handle_traits>;

	constexpr bool operator==(const kernel_handle& lhs, const kernel_handle& rhs) noexcept
	{ return lhs.equals(rhs); }

	constexpr bool operator!=(const kernel_handle& lhs, const kernel_handle& rhs) noexcept
	{ return !(lhs == rhs); }

	template <typename LeftClose, typename RightClose>
	constexpr bool operator==(const unsafe_handle<LeftClose>& lhs, const unsafe_handle<RightClose>& rhs) noexcept
	{ return lhs.equals(rhs); }

	template <typename LeftClose, typename RightClose>
	constexpr bool operator!=(const unsafe_handle<LeftClose>& lhs, const unsafe_handle<RightClose>& rhs) noexcept
	{ return !(lhs == rhs); }

} // end namespace distant

#include "impl/unsafe_handle.hxx"

  // Remarks:
  //		Process-local handle table starts at entry 4, hence the null ( == 0) 
  // entry is not a valid one. WINAPI functions tend to return NULL, though some
  // of them return INVALID_HANDLE_VALUE.