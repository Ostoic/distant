#pragma once
#include <distant\kernel\process.hpp>

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

/// \file Implementation header of distant::kernel::process

#include <distant\wait.hpp>

#include <distant\support\winapi\privilege.hpp>

namespace distant::kernel {

//public:
	template <access_rights::process T>
	inline void process<T>::terminate() const
	{
		using access_rights = access_rights::process;

		static_assert(
			check_permission(access_rights::terminate),
			"Invalid access_rights (process::terminate): "
			"Process must have terminate access right");

		process_base::terminate();
	}

	// Check if we have permission to perform the given action
	template <access_rights::process T>
	inline constexpr bool process<T>::check_permission(flag_type access) noexcept
	{ 
		return (T & access) == access; 
	}

	template <access_rights::process T>
	inline bool process<T>::is_active() const 
	{
		using access_rights = access_rights::process;

		// Ensure we have the synchronize access_rights
		// This is required to call WaitForSingleObject
		static_assert(
			check_permission(access_rights::synchronize),
			"Invalid access rights (process::is_active): "
			"Process must have synchronize access right");

		return process_base::is_active();
	}

	template <access_rights::process T>
	inline std::string process<T>::filename() const
	{
		using access_rights = access_rights::process;

		static_assert(
			check_permission(access_rights::query_information) ||
			check_permission(access_rights::query_limited_information),
			"Invalid access rights (process::name): "
			"Process must have query_information or query_limited_information access rights");

		return process_base::filename();
	}

	template <access_rights::process T>
	inline distant::filesystem::path process<T>::file_path() const
	{
		using access_rights = access_rights::process;

		static_assert(
			check_permission(access_rights::query_information) ||
			check_permission(access_rights::query_limited_information),
			"Invalid access rights (process::file_path): "
			"Process must have query_information or query_limited_information access rights");
		
		return process_base::file_path();
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
	inline process<T>::process(pid_type id) noexcept
		: process_base(id, T) {}

	// Take possession of process handle. It is ensured to be a convertible process handle
	// due to encoded type in handle.x
	template <access_rights::process T>
	inline process<T>::process(handle<process>&& handle) noexcept
		: process_base(std::move(handle), T){}											

	// XXX Choose weakest access rights or produce error about incompatible access rights
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
	inline process<access_rights::process::all_access> 
	current_process() noexcept
	{
		const auto native_handle = boost::winapi::GetCurrentProcess();
		const auto pid = boost::winapi::GetProcessId(native_handle);
		return process<>(pid);
	}

} // end namespace distant::kernel
