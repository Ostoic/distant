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
		auto native_handle = ::GetCurrentProcess();
		auto pid = ::GetProcessId(native_handle);
		return process(pid);
	}

//protected:
	// Type-safe version of OpenProcess
	template <access_rights::process T>
	inline typename process<T>::handle_type process<T>::open(pid_type id)
	{
		using flag_t = std::underlying_type_t<flag_type>;
	
		if (id != 0)
		{
			auto result = ::OpenProcess(static_cast<flag_t>(T), false, id);
			if (result != NULL)
				return handle_type(result); // Returns windows::handle with result as value
		}

		return windows::invalid_handle;
	}

	/*template <access_rights::process T>
	inline void process<T>::terminate(const handle_type& h)
	{
		static_assert(
			check_permission(access_rights::terminate),
			"Invalid access_rights (process::terminate): "
			"Process must have terminate access right");

		unsigned int exit_code = 0;
		auto native_handle = expose::native_handle(h);
		TerminateProcess(native_handle, exit_code);
		return;
	}*/

	template <access_rights::process T>
	inline typename process<T>::pid_type process<T>::get_pid(const handle_type& h)
	{
		auto native_handle = expose::native_handle(h);
		auto id = GetProcessId(native_handle);
		return static_cast<pid_type>(id);
	}

	//template <access_rights::process T>
	//inline std::string process<T>::get_file_path(const handle_type& h)
	//{
	//	static_assert(
	//		check_permission(access_rights::query_information) ||
	//		check_permission(access_rights::query_limited_information),
	//		"Invalid access rights (process::get_file_path): "
	//		"Process must have query_information or query_limited_information access rights");
	//	auto native_handle = expose::native_handle(h);
	//	char out_string[MAX_PATH] = "";
	//	DWORD max_path = MAX_PATH;

	//	// 0 indicates: The name should use the Win32 path format.
	//	QueryFullProcessImageNameA(native_handle, 0, out_string, &max_path);
	//	return std::string(out_string);
	//}

	/*template <access_rights::process T>
	inline std::size_t process<T>::get_handle_count(const handle_type& h)
	{
		static_assert(
			check_permission(access_rights::query_information) ||
			check_permission(access_rights::query_limited_information),
			"Invalid access rights (process::get_handle_count): "
			"Process must have query_information or query_limited_information access rights");

		auto native_handle = expose::native_handle(h);

		DWORD count = 0;
		GetProcessHandleCount(native_handle, &count);
		return static_cast<std::size_t>(count);
	}*/

//public:
	//====================================//
	// Process id and access_rights flags //
	// accessors                          //
	//====================================//
	// Check if the process handle is valid
	template <access_rights::process T>
	inline bool process<T>::valid()  const
	{
		return this->weakly_valid() &&
			get_pid() != std::numeric_limits<pid_type>::infinity();
	}

	template <access_rights::process T>
	inline void process<T>::terminate() const
	{
		static_assert(
			check_permission(access_rights::terminate),
			"Invalid access_rights (process::terminate): "
			"Process must have terminate access right");

		if (!this->valid())
			//throw std::invalid_argument("Invalid process handle");
		{
			this->set_last_error(ERROR_INVALID_HANDLE);
			return;
		}

		unsigned int exit_code = 0;
		auto native_handle = expose::native_handle(m_handle);
		if (!::TerminateProcess(native_handle, exit_code))
			this->update_gle();
		else
			this->set_success();
		return;
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

		using windows::wait;
		if (!this->valid())
			//throw std::invalid_argument("Invalid process handle");
		{
			this->set_last_error(ERROR_INVALID_HANDLE);
			return false;
		}

		wait wait_for;

		// Return immediately
		auto result = wait_for(*this, 0);
		this->update_gle(wait_for);
		return result == wait::state::timeout;
	}

	template <access_rights::process T>
	inline std::size_t process<T>::get_handle_count() const
	{
		static_assert(
			check_permission(access_rights::query_information) ||
			check_permission(access_rights::query_limited_information),
			"Invalid access rights (process::get_handle_count): "
			"Process must have query_information or query_limited_information access rights");

		if (!this->valid())
			//throw std::invalid_argument("Invalid process handle");
		{
			this->set_last_error(ERROR_INVALID_HANDLE);
			return 0;
		}

		auto native_handle = expose::native_handle(m_handle);

		DWORD count = 0;
		if (!::GetProcessHandleCount(native_handle, &count))
			this->update_gle();
		else
			this->set_success();
		
		return static_cast<std::size_t>(count);
	}

	template <access_rights::process T>
	inline std::string process<T>::get_file_path() const 
	{
		static_assert(
			check_permission(access_rights::query_information) ||
			check_permission(access_rights::query_limited_information),
			"Invalid access rights (process::get_file_path): "
			"Process must have query_information or query_limited_information access rights");

		if (!this->valid())
			//throw std::invalid_argument("Invalid process handle");
		{
			this->set_last_error(ERROR_INVALID_HANDLE);
			return "";
		}

		auto native_handle = expose::native_handle(m_handle);

		char out_string[MAX_PATH] = "";
		DWORD max_path = MAX_PATH;

		// 0 indicates: The name should use the Win32 path format.
		if (!::QueryFullProcessImageNameA(native_handle, 0, out_string, &max_path))
			this->update_gle();
		else
			this->set_success();
		
		return std::string(out_string);
	}

	template <access_rights::process T>
	system::process_memory process<T>::get_memory_status() const
	{
		return system::process_memory(*this);
	}

	template <access_rights::process T>
	inline const windows::handle<process<T>>& process<T>::get_handle() const 
	{ 
		return object::get_handle<process<T>>(); 
	}

	//=========================//
	// Process ctors and dtor  //
	//=========================//
	// Empty initialize process
	template <access_rights::process T>
	inline constexpr process<T>::process() :
		base_type(), // Empty initialize object
		m_pid(std::numeric_limits<pid_type>::infinity()),
		m_access(T) {}

	// Open process by id
	template <access_rights::process T>
	inline process<T>::process(pid_type id) :
		base_type(this->open(id)),
		m_pid(id),
		m_access(T)
	{ this->update_gle(); }

	// Take possession of process handle. It is ensured to be a convertible process handle
	// due to encoded type in windows::handle.
	template <access_rights::process T>
	inline process<T>::process(handle_type&& handle) :
		base_type(std::move(handle)),			// steal handle
		m_access(T)					// steal access flags
	{ m_pid = get_pid(get_handle()); }			// retrieve process id
												// This is done after initialization to ensure the operation
												// is performed after moving handle into our possession.

	template <access_rights::process T>
	inline process<T>::process(process<T>&& other) :
		base_type(std::move(other)),
		m_pid(std::move(other.m_pid)),
		m_access(std::move(other.m_access)) {} // XXX Choose weakest access rights or produce error about incompatible access rights
	
	template <access_rights::process T>
	inline process<T>& process<T>::operator=(process<T>&& other)
	{
		base_type::operator=(std::move(other));
		m_access = other.m_access; // XXX Choose weakest access rights
		m_pid = other.m_pid;
		return *this;
	}

//private:
	// Close process handle and invalidate process object
	// Mutates: from invalidate() 
	template <access_rights::process T>
	inline void process<T>::close_process()
	{
		if (this->valid())
		{
			this->close_object();
			this->invalidate();
		}
	}

	// Process destructor: Clean up handles and invalidate interior data.
	// Call Chain:
	//		1. ~process()
	//		2. ~securable()
	//		3. ~object()
	//		4. ~handle() <-- calls CloseHandle
	// Mutates: from invalidate() 
	template <access_rights::process T>
	inline process<T>::~process() 
	{
		this->invalidate(); 
	}

	template <access_rights::process T>
	process<T>::operator bool() const
	{ 
		return this->valid(); 
	}

	// Invalidate process id and handle
	// Mutates: m_id, m_handle
	template <access_rights::process T>
	inline void process<T>::invalidate() 
	{ 
		m_pid = std::numeric_limits<pid_type>::infinity(); 
	}

//free:
	template <access_rights::process T, access_rights::process U>
	inline bool operator ==(const process<T>& lhs, const process<U>& rhs)
	{
		return lhs.m_handle == rhs.m_handle &&
			   lhs.m_pid	== rhs.m_pid;    
			   //lhs.m_access == rhs.m_access;
	}

	template <access_rights::process T, access_rights::process U>
	inline bool operator !=(const process<T>& lhs, const process<U>& rhs)
	{
		return !operator==(lhs, rhs);
	}

} // end namespace distant::windows::kernel
