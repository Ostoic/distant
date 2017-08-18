#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

// Implementation header of distant::windows::kernel::process
#include <distant\windows\kernel\process.hpp>
#include <distant\windows\wait.hpp>

namespace distant::windows::kernel {

//public:
	//===========================//
	// Static process functions //
	//===========================//
	template <access_rights::process T>
	inline process<T> process<T>::get_current()
	{
		const auto native_handle = ::GetCurrentProcess();
		const auto pid = ::GetProcessId(native_handle);
		return process{ pid };
	}

//protected:

//public:
	//===================//
	// Process interface //
	//===================//

	template <access_rights::process T>
	inline void process<T>::terminate() const
	{
		static_assert(
			check_permission(access_rights::terminate),
			"Invalid access_rights (process::terminate): "
			"Process must have terminate access right");

		process_base::terminate();
	}

	// Check if we have permission to perform the given action
	template <access_rights::process T>
	inline constexpr bool process<T>::check_permission(flag_type access)
	{ 
		return (T & access) == access; 
	}

	template <access_rights::process T>
	inline bool process<T>::is_active() const
	{
		// Ensure we have the synchronize access_rights
		// This is required to call WaitForSingleObject
		static_assert(
			check_permission(access_rights::synchronize),
			"Invalid access rights (process::is_running): "
			"Process must have synchronize access right");

		return process_base::is_active();
	}

	template <access_rights::process T>
	inline auto process<T>::name() const
	{
		static_assert(
			check_permission(access_rights::query_information) ||
			check_permission(access_rights::query_limited_information),
			"Invalid access rights (process::file_path): "
			"Process must have query_information or query_limited_information access rights");

		return process_base::name();
	}

	template <access_rights::process T>
	inline std::string process<T>::file_path() const
	{
		static_assert(
			check_permission(access_rights::query_information) ||
			check_permission(access_rights::query_limited_information),
			"Invalid access rights (process::file_path): "
			"Process must have query_information or query_limited_information access rights");
<<<<<<< HEAD
		
		return process_base::file_path();
	}

	template <access_rights::process T>
	inline auto process<T>::memory_status() const
	{
		return memory_status_t{ *this };
	}

	template <access_rights::process T>
	inline const windows::handle<process<T>>& 
	process<T>::get_handle() const 
	{ 
		return object::get_handle<process<T>>(); 
	}

	//=========================//
	// Process ctors and dtor  //
	//=========================//
	// Empty initialize process
	template <access_rights::process T>
	inline constexpr process<T>::process()
		: process_base() {}

	// Open process by id
	template <access_rights::process T>
	inline process<T>::process(pid_type id) 
		: process_base(id, T) {}

	// Take possession of process handle. It is ensured to be a convertible process handle
	// due to encoded type in windows::handle.
	template <access_rights::process T>
	inline process<T>::process(handle_type&& handle) 
		: process_base(std::move(handle), T){}											

	// XXX Choose weakest access rights or produce error about incompatible access rights
	template <access_rights::process T>
	inline process<T>::process(process<T>&& other) 
		: process_base(std::move(other)), {} 
	
	template <access_rights::process T>
	inline process<T>& process<T>::operator=(process<T>&& other)
	{
		process_base::operator=(std::move(other));
		return *this;
	}

//free:
	template <access_rights::process T, access_rights::process U>
	inline bool operator ==(const process<T>& lhs, const process<U>& rhs) { return operator==(lhs, rhs); }

	template <access_rights::process T, access_rights::process U>
	inline bool operator !=(const process<T>& lhs, const process<U>& rhs) { return !operator==(lhs, rhs); }

} // end namespace distant::windows::kernel
