// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once

#include <distant/config.hpp>
#include <distant/access_rights.hpp>
#include <distant/type_traits.hpp>

#include <distant/concepts/boolean_validator.hpp>
#include <boost/operators.hpp>

#include <boost/winapi/config.hpp>
#include <boost/winapi/basic_types.hpp>
#include <boost/winapi/handles.hpp>

#include <bitset>
#include <memory>

namespace distant
{
	namespace detail
	{
		template <class HandleTraits>
		struct scoped_handle_deleter
		{
			template <class Handle>
			void operator()(const Handle handle) noexcept
			{
				HandleTraits::close(handle);
			}
		};
	}

	struct kernel_handle_traits
	{
		using native_t = boost::winapi::HANDLE_;
		static constexpr native_t invalid_handle = nullptr;
		static constexpr native_t null_handle = nullptr;

		static bool close(const native_t native_handle) noexcept
		{
			if (native_handle == reinterpret_cast<native_t>(-1))
				return true;

			const auto result = boost::winapi::CloseHandle(native_handle);
			BOOST_ASSERT_MSG(result, "[kernel_handle_traits::close] Invalid handle specified");
			return result;
		}
	};

	template <class HandleTraits = kernel_handle_traits>
	class scoped_handle
		: public concepts::boolean_validator<scoped_handle<HandleTraits>>
		, public boost::equality_comparable<scoped_handle<HandleTraits>>
	{
	public:
		// Underlying handle type. This is macro'd in Windows to be void* == (HANDLE)
		using native_t = typename handle_traits<scoped_handle>::native_t;
		using traits_t = HandleTraits;
		using flag_t = access_rights::handle;

	public:
		/// Construct using native handle.
		/// @param h the native handle value
		/// @param flags handle flags
		explicit constexpr scoped_handle(native_t h, flag_t flags = flag_t::inherit, bool closed = false) noexcept;

		/// Construct an invalid handle.
		/// This allows handles to be comparable with nullptr.
		/// @param h the nullptr.
		constexpr scoped_handle(nullptr_t h) noexcept;

		/// Construct invalid handle.
		/// This calls the nullptr constructor.
		constexpr scoped_handle() noexcept;

		/// Move copyable
		scoped_handle(scoped_handle&&) noexcept;

		/// Move assignable
		scoped_handle& operator=(scoped_handle&&) noexcept;

		// scoped_handle is meant to be a cheap wrapper around
		scoped_handle(const scoped_handle&) = delete;
		scoped_handle& operator =(const scoped_handle&) = delete;

		/// Close handle to windows object.
		/// Handle must be weakly valid in order to close the handle.
		~scoped_handle() noexcept { this->close(); }

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
		constexpr native_t native_handle() const noexcept;

		template <class CloseL, class CloseR>
		friend constexpr bool operator==(const scoped_handle<CloseL>& lhs, const scoped_handle<CloseR>& rhs) noexcept
		{ return lhs.native_handle_ == rhs.native_handle_; }

		friend bool operator==(const scoped_handle& lhs, const scoped_handle& rhs) noexcept
		{ return lhs.native_handle_ == rhs.native_handle_; }

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
		//native_t native_handle_;
		// Using unique_ptr<void> with a custom deleter does not lead to dynamic allocations!
		std::unique_ptr<std::remove_pointer_t<native_t>, detail::scoped_handle_deleter<HandleTraits>> native_handle_;

		// If we somehow attempt to call CloseHandle multiple times,
		// this will help prevent further unnecessary calls.
		/// Switch to check if closure was observed
		std::bitset<3> flags_;

		template <class> friend class scoped_handle;
	};

	template <typename Traits>
	struct handle_traits<scoped_handle<Traits>> : Traits {};

	using kernel_handle = scoped_handle<kernel_handle_traits>;

	/*constexpr bool operator==(const kernel_handle& lhs, const kernel_handle& rhs) noexcept
	{ return lhs.native_handle() == rhs.native_handle(); }*/

} // end namespace distant

#include "impl/scoped_handle.hxx"

  // Remarks:
  //		Process-local handle table starts at entry 4, hence the null ( == 0)
  // entry is not a valid one. WINAPI functions tend to return NULL, though some
  // of them return INVALID_HANDLE_VALUE.