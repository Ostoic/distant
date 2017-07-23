#pragma once

/*!
@file
Includes all the library components except the adapters for external
libraries.

@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

// Implementation header of distant::windows::kernel::process

#include <distant\windows\kernel\process.hpp>
#include <distant\windows\wait.hpp>

#include <distant\utility\attorney.hpp> // For extracting window::handle value_type

namespace distant {
namespace windows {
namespace kernel  {
	//===========================//
	// Static process functions //
	//===========================//
	template <access_rights::process T>
	process<T> process<T>::get_current()
	{
		process<T> current(static_cast<windows::handle>(GetCurrentProcess()));
		return current;
	}
	
	// Type-safe version of OpenProcess
	template <access_rights::process T>
	typename process<T>::handle_type process<T>::open(id_type id)
	{
		using flag_t = std::underlying_type_t<flag_type>;
		using handle_value = windows::handle::value_type;
	
		if (id != 0)
		{
			handle_value result = OpenProcess(static_cast<flag_t>(T), false, id);
			if (result != NULL)
				return windows::handle(result); // Returns windows::handle with result as value
		}

		return windows::invalid_handle;
	}

	template <access_rights::process T>
	typename process<T>::id_type process<T>::get_pid(const handle_type& h)
	{
		if (h == distant::invalid_handle)
		{
			//throw
		}

		auto handle_value = utility::attorney::to_handle<process>::get_value(h);
		auto id = GetProcessId(handle_value);
		return static_cast<id_type>(id);
	}

	template <access_rights::process T>
	std::size_t process<T>::get_handle_count(const handle_type& h)
	{
		static_assert(
			check_permission_t<access_rights::process::query_information>::value ||
			check_permission_t<access_rights::process::query_limited_information>::value,
			"Invalid access_rights (distant::process::get_handle_count): "
			"Process must have query_information or query_limited_information access rights");

		DWORD count = 0;

		auto handle_value = utility::attorney::to_handle<process>::get_value(h);
		GetProcessHandleCount(handle_value, &count);
		return static_cast<std::size_t>(count);
	}

	/*template <access_rights::process T>
	error_code_type process<T>::terminate(const handle_type& h)
	{
		static_assert(
			check_permission_t<access_rights::process::terminate>::value,
			"Invalid access_rights (distant::process::terminate): "
			"Process must have terminate access");

		UINT exit_code = 0;

		auto handle_value = detail::attorney::to_handle<process>::get_value(h);
		TerminateProcess(handle_value, &exit_code);
		return static_cast<>(exit_code);
	}*/


	//====================================//
	// Process id and access_rights flags //
	// accessors                          //
	//====================================//
	// Check if the process handle is valid
	template <access_rights::process T>
	bool process<T>::valid()  const
	{
		return this->weakly_valid() &&
			get_id() != std::numeric_limits<id_type>::infinity();
	}

	// Check if we have permission perform the given action
	template <access_rights::process T>
	constexpr bool process<T>::check_permission(flag_type access) const
	{
		return (T & access) == access;
	}

	template <access_rights::process T>
	bool process<T>::is_running() const
	{
		// Ensure we have the synchronize access_rights
		// This is required to call WaitForSingleObject
		static_assert(
			process<T>::check_permission_t<access_rights::process::synchronize>::value,
			"Invalid access_rights (distant::process::is_running): "
			"Process must have synchronize access rights");

		using windows::wait;
		if (!this->valid()) return false;

		wait wait_for;
		wait::state state = wait::state::abandoned;

		state = wait_for(*this, 0);
		this->update_gle(wait_for);
		return state == wait::state::timeout;
	}

	template <access_rights::process T>
	std::size_t process<T>::get_handle_count() const
	{
		auto count = get_handle_count(m_handle);
		this->update_gle();
		return count;
	}

	//=========================//
	// Process ctors and dtor  //
	//=========================//
	// Empty initialize process
	template <access_rights::process T>
	constexpr process<T>::process() :
		object_type(), // Empty initialize synchro
		m_id(std::numeric_limits<id_type>::infinity()),
		m_access(T)
	{}

	// Open process by id
	template <access_rights::process T>
	process<T>::process(id_type id) :
		object_type(this->open(id)),
		m_id(id),
		m_access(T)
	{ this->update_gle(); }

	// Take possession of process handle
	// NOTE: This is sort of type-unsafe since the handle could be a handle
	// to any kernel object.
	template <access_rights::process T>
	process<T>::process(handle_type&& handle) :
		object_type(std::move(handle)), // steal handle
		m_access(T)						// steal access flags
	{ m_id = get_pid(m_handle); }		// retrieve process id
										// This is done after initialization to ensure the operation
										// is performed after moving handle into our possession.

	template <access_rights::process T>
	process<T>::process(process<T>&& other) :
		securable(std::move(other)),
		m_id(std::move(other.m_id)),
		m_access(std::move(other.m_access)) // XXX Choose weakest access rights
	{}

	template <access_rights::process T>
	process<T>& process<T>::operator=(process<T>&& other)
	{
		object_type::operator=(std::move(other));
		m_access = other.m_access; // XXX Choose weakest access rights
		m_id = other.m_id;
		return *this;
	}

	// Close process handle and invalidate process object
	// Mutates: from invalidate() 
	template <access_rights::process T>
	void process<T>::close_process()
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
	process<T>::~process() { this->invalidate(); }

	// Invalidate process id and handle
	// Mutates: m_id, m_handle
	template <access_rights::process T>
	void process<T>::invalidate() { m_id = std::numeric_limits<id_type>::infinity(); }

} // end namespace kernel
} // end namespace windows
} // end namespace distant