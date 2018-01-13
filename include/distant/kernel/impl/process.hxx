#pragma once
#include <distant\kernel\process.hpp>

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

/// \file Implementation header of distant::kernel::process

#include <limits>

#include <distant\wait.hpp>

#include <distant\support\winapi\privilege.hpp>

namespace distant::kernel {

//public:
	template <access_rights::process T>
	inline void process<T>::kill()
	{
		using access_rights = access_rights::process;

		// The TerminateProcess API call requires the following access rights.
		static_assert(
			check_permission(T, access_rights::terminate),
			"Invalid access_rights (process::terminate): "
			"Process must have terminate access right");

		process_base::kill();
	}

	template <access_rights::process T>
	inline bool process<T>::is_active() const 
	{
		using access_rights = access_rights::process;

		// The WaitForSingleObject API call requires the following access rights.
		static_assert(
			check_permission(T, access_rights::synchronize),
			"Invalid access rights (process::is_active): "
			"Process must have synchronize access right");

		return process_base::is_active();
	}
	
	template <access_rights::process T>
	inline bool process<T>::is_32bit() const
	{
		using access_rights = access_rights::process;

		// The IsWow64Process API call requires the following access rights.
		static_assert(
			check_permission(T, access_rights::query_information) ||
			check_permission(T, access_rights::query_limited_information),
			"Invalid access rights (process::filename): "
			"Process must have query_information or query_limited_information access rights");

		return process_base::is_32bit();
	}

	template <access_rights::process T>
	inline std::wstring process<T>::filename() const
	{
		using access_rights = access_rights::process;

		// The QueryFullProcessImageName API call requires the following access rights.
		static_assert(
			check_permission(T, access_rights::query_information) ||
			check_permission(T, access_rights::query_limited_information),
			"Invalid access rights (process::filename): "
			"Process must have query_information or query_limited_information access rights");

		return process_base::filename();
	}

	template <access_rights::process T>
	inline filesystem::path process<T>::file_path() const
	{
		using access_rights = access_rights::process;

		// The QueryFullProcessImageName API call requires the following access rights.
		static_assert(
			check_permission(T, access_rights::query_information) ||
			check_permission(T, access_rights::query_limited_information),
			"Invalid access rights (process::file_path): "
			"Process must have query_information or query_limited_information access rights");
		
		return process_base::file_path();
	}

	template <access_rights::process T>
	template <access_rights::process Other_Flag, typename>
	inline process<T>::operator const process<Other_Flag>&() const noexcept
	{
		return reinterpret_cast<const process<Other_Flag>&>(*this);
	}

	//=========================//
	// Process ctors and dtor  //
	//=========================//
	// Empty initialize process
	template <access_rights::process T>
	inline constexpr process<T>::process() noexcept
		: process_base() {}

	// Open process by id
	template <access_rights::process T>	
	inline process<T>::process(std::size_t id) noexcept
		: process_base(id, T) {}

	// Take possession of process handle. It is ensured to be a convertible process handle
	// due to encoded type in handle.
	template <access_rights::process T>
	inline process<T>::process(handle<process>&& handle) noexcept
		: process_base(std::move(handle), T){}											

	template <access_rights::process T>
	inline process<T>::process(process<T>&& other) noexcept
		: process_base(std::move(other)) {} 
	
	template <access_rights::process T>
	inline process<T>& process<T>::operator=(process<T>&& other) noexcept
	{
		process_base::operator=(std::move(other));
		return *this;
	}

//free:
	template <access_rights::process T = access_rights::process::all_access>
	process<T> current_process() noexcept
	{
		return reinterpret_cast<process<T>&&>(current_process());
	}


} // end namespace distant::kernel