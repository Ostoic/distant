// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once
#include <distant/kernel_objects/process.hpp>

/***
 * @file Contains the implementation of distant::process.
 */

namespace distant::kernel_objects
{
//class process
//public:
	template <access_rights::process T>
	template <typename Return>
	auto process<T>::kill()
		-> require_permission<process_rights::terminate, Return>
	{
		// The TerminateProcess API call requires the following access rights.
		static_assert(
			check_permission(T, process_rights::terminate),
			"Invalid access_rights (process::terminate): "
			"Process must have terminate access right"
		);

		unsafe_process::kill();
		return;
	}

	template <access_rights::process T>
	template <typename Return>
	auto process<T>::join()
		-> require_permission<process_rights::synchronize, Return>
	{
		// The WaitForSingleObjectEx API call requires the following access rights.
		static_assert(
			check_permission(T, process_rights::synchronize),
			"Invalid access_rights (process::synchronize): "
			"Process must have synchronize access right"
		);

		unsafe_process::join();
		return;
	}

	template <access_rights::process T>
	template <typename Return>
	auto process<T>::handle_count() const
		-> require_permission<process_rights::query_limited_information, Return>
	{
		// The TerminateProcess API call requires the following access rights.
		static_assert(
			check_permission(T, process_rights::query_limited_information),
			"Invalid access_rights (process::terminate): "
			"Process must have terminate access right"
		);

		return unsafe_process::handle_count();
	}

	template <access_rights::process T>
	template <typename Return>
	auto process<T>::is_active() const
		-> require_permission<process_rights::synchronize, Return>
	{
		using access_rights = access_rights::process;

		// The WaitForSingleObject API call requires the following access rights.
		static_assert(
			check_permission(T, access_rights::synchronize),
			"[process::is_active] Invalid access rights: "
			"Process must have syncronize access right"
		);

		return unsafe_process::is_active();
	}

	template <access_rights::process T>
	template <typename Return>
	auto process<T>::is_32bit() const
		-> require_permission<process_rights::query_information | process_rights::query_limited_information, Return>
	{
		// The IsWow64Process API call requires the following access rights.
		static_assert(
			check_permission(T, process_rights::query_information) ||
			check_permission(T, process_rights::query_limited_information),
			"[process::is_32bit] Invalid access rights: "
			"Process must have query_information or query_limited_information access rights"
		);

		return unsafe_process::is_32bit();
	}

	template <access_rights::process T>
	template <typename Return>
	auto process<T>::is_64bit() const
		-> require_permission<process_rights::query_information | process_rights::query_limited_information, Return>
	{
		// The IsWow64Process API call requires the following access rights.
		static_assert(
			check_permission(T, process_rights::query_information) ||
			check_permission(T, process_rights::query_limited_information),
			"[process::is_64bit] Invalid access rights: "
			"Process must have query_information or query_limited_information access rights"
		);

		return unsafe_process::is_64bit();
	}

	template <access_rights::process T>
	template <typename Return>
	auto process<T>::name() const
		-> require_permission<process_rights::query_information | process_rights::query_limited_information, Return>
	{
		// The QueryFullProcessImageName API call requires the following access rights.
		static_assert(
			check_permission(T, process_rights::query_information) ||
			check_permission(T, process_rights::query_limited_information),
			"Invalid access rights (process::name): "
			"Process must have query_information or query_limited_information access rights"
		);

		return unsafe_process::name();
	}

	template <access_rights::process T>
	template <typename Return>
	auto process<T>::file_path() const
		-> require_permission<process_rights::query_information | process_rights::query_limited_information, Return>
	{
		// The QueryFullProcessImageName API call requires the following access rights.
		static_assert(
			check_permission(T, process_rights::query_information) ||
			check_permission(T, process_rights::query_limited_information),
			"Invalid access rights (process::file_path): "
			"Process must have query_information or query_limited_information access rights"
		);

		return unsafe_process::file_path();
	}

	template <access_rights::process T>
	template <process_rights OtherAccess, typename>
	process<T>::operator process<OtherAccess>&() noexcept
	{ return *reinterpret_cast<process<OtherAccess>*>(this); }

	template <access_rights::process T>
	template <process_rights OtherAccess, typename>
	process<T>::operator const process<OtherAccess>&() const noexcept
	{ return *reinterpret_cast<const process<OtherAccess>*>(this); }

//=========================//
// Process ctors and dtor  //
//=========================//
// Empty initialize process
	template <access_rights::process T>
	constexpr process<T>::process() noexcept
		: unsafe_process()
	{}

	// Open process by id
	template <access_rights::process T>
	process<T>::process(const id_t id) noexcept
		: unsafe_process(id, T)
	{}

	template <access_rights::process T>
	process<T>::process(process&& other) noexcept
		: unsafe_process(std::move(other))
	{}

	template <access_rights::process T>
	process<T>& process<T>::operator=(process&& other) noexcept
	{
		unsafe_process::operator=(std::move(other));
		return *this;
	}

//private:
	template <access_rights::process T>
	process<T>::process(kernel_handle&& handle) noexcept
		: unsafe_process(std::move(handle), T)
	{}

//free:
	template <access_rights::process T>
	process<T> current_process() noexcept
	{
		return reinterpret_cast<process<T>&>(current_process());
	}

	inline process<> current_process() noexcept
	{
		return process<>{kernel_handle{
			GetCurrentProcess() ,
			access_rights::handle::close_protected
		}};
	}

} // end namespace distant::kernel_objects
