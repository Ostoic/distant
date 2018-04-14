// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/access_rights.hpp>

#include <distant/detail/attorney.hpp>
#include <distant/utility/boolean_validator.hpp>

#include <boost/winapi/config.hpp>
#include <boost/winapi/basic_types.hpp>

#include <bitset>

namespace distant {
namespace detail  {

	// Forward declare invalid handle literal
	class invalid_t;

	// Implements the interface of handle
	class handle_base : public utility::boolean_validator<handle_base>
	{
	public:
		// Underlying handle type. This is macro'd in Windows to be void* == (HANDLE)
		using native_type = boost::winapi::HANDLE_;
		using flag_type = access_rights::handle;

	public:
		/// Construct using native handle.
		/// @param h the native handle value
		/// @param flags handle flags 
		explicit constexpr handle_base(native_type h, flag_type flags = flag_type::inherit, bool closed = false) noexcept;

		/// Construct an invalid handle.
		/// This allows handles to be comparable with nullptr.
		/// @param h the nullptr.
		constexpr handle_base(nullptr_t h) noexcept;

		/// Construct invalid handle.
		/// This calls the nullptr constructor.
		constexpr handle_base() noexcept;

		/// Move copyable
		handle_base(handle_base&&) noexcept;

		/// Move assignable
		handle_base& operator=(handle_base&&) noexcept;

		// If we allow copy ctor/assignment, then multiple copies will eventually attempt 
		// to close the same handle, which is not desirable.
		handle_base(const handle_base&) = delete;
		handle_base& operator =(const handle_base&) = delete;

		/// Close handle to windows object.
		/// Handle must be weakly valid in order to close the handle.
		~handle_base() noexcept { this->close(); }
		
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
		native_type native_handle() const noexcept;

	protected:
		// From "Windows Via C\C++" by Jeffrey Richter,
		// setting the handle to null is preferable to invalid_handle
		// after closing the handle. This is probably because some API
		// calls consider invalid_handle as the current process.
		/// Numerically invalidate and close protect our handle.
		void invalidate() noexcept;

		/// Protect the handle from being closed
		void protect() noexcept;

		/// Get the handle's flag type
		/// @return distant::access_rights::handle flag type
		flag_type flags() const noexcept;

	protected:
		/// native HANDLE value
		native_type native_handle_;

		// If we somehow attempt to call CloseHandle multiple times,
		// this will help prevent further unnecessary calls.
		/// Switch to check if closure was observed 
		std::bitset<3> flags_;
		
	public:
		friend constexpr bool operator ==(const handle_base&, const handle_base&) noexcept;
		friend constexpr bool operator !=(const handle_base&, const handle_base&) noexcept;
	};

} // end namespace detail
} // end namespace distant

#include <distant/impl/handle_base.hxx>

  // Remarks:
  //		Process-local handle table starts at entry 4, hence the null ( == 0) 
  // entry is not a valid one. WINAPI functions tend to return NULL, though some
  // of them return INVALID_HANDLE_VALUE.