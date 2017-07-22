#pragma once
//#include <stdafx.h>

#include <distant\process.h>
#include <distant\windows\wait.h>


namespace distant {

	//===========================//
	// Static process functions //
	//===========================//
	template <windows::access_rights T>
	process<T> process<T>::get_current()
	{
		process<T> current(static_cast<windows::handle>(GetCurrentProcess()));
		return current;
	}
	
	// Type-safe version of OpenProcess
	template <windows::access_rights T>
	typename process<T>::handle_type process<T>::open(id_type id)
	{
		using flag_t = std::underlying_type_t<flag_type>;
		using handle_value = windows::handle::value_type;
	
		if (id != 0)
		{
			handle_value result = OpenProcess(static_cast<flag_t>(T), false, id);
			return windows::handle(result); // Returns windows::handle with result as value
		}

		return windows::invalid_handle;
	}

	template <windows::access_rights T>
	typename process<T>::id_type process<T>::get_pid(const handle_type& h)
	{
		auto handle_value = detail::attorney::to_handle<process>::get_value(h);
		auto id = GetProcessId(handle_value);
		return static_cast<id_type>(id);
	}

	//====================================//
	// Process id and access_rights flags //
	// accessors                          //
	//====================================//
	// Check if the process handle is valid
	template <windows::access_rights T>
	bool process<T>::valid()  const
	{
		return this->weakly_valid() &&
			get_id() != std::numeric_limits<id_type>::infinity();
	}

	// Check if we have permission perform the given action
	template <windows::access_rights T>
	constexpr bool process<T>::check_permission(flag_type access) const
	{
		return (T & access) == access;
	}

	template <windows::access_rights T>
	bool process<T>::is_running() const
	{
		// Ensure we have the synchronize access_rights
		// This is required to call WaitForSingleObject
		static_assert(
			check_permission_t<windows::access_rights::synchronize>::value,
			"Invalid access_rights: Process must have synchronize access rights");

		using windows::wait;
		if (!this->valid()) return false;

		wait wait_for;
		wait::state state = wait::state::abandoned;

		state = wait_for(*this);
		//this->update_gle(wait_for);
		return state == wait::state::timeout;
	}

	//=========================//
	// Process ctors and dtor  //
	//=========================//
	// Empty initialize process
	template <windows::access_rights T>
	constexpr process<T>::process() :
		object_type(), // Empty initialize synchro
		m_id(std::numeric_limits<id_type>::infinity()),
		m_access(T)
	{}

	// Open process by id
	template <windows::access_rights T>
	process<T>::process(id_type id) :
		object_type(this->open(id)),
		m_id(id),
		m_access(T)
	{ this->update_gle(); }

	// Take possession of process handle
	// NOTE: This is sort of type-unsafe since the handle could be a handle
	// to any kernel object.
	template <windows::access_rights T>
	process<T>::process(handle_type&& handle) :
		object_type(std::move(handle)), // steal handle
		m_access(T)					// steal access flags
	{ m_id = get_pid(m_handle); }		// retrieve process id
										// This is done after initialization to ensure the operation
										// is performed after moving handle into our possession.

	template <windows::access_rights T>
	process<T>::process(process<T>&& other) :
		securable(std::move(other)),
		m_id(std::move(other.m_id)),
		m_access(std::move(other.m_access)) // XXX Choose weakest access rights
	{}

	template <windows::access_rights T>
	process<T>& process<T>::operator=(process<T>&& other)
	{
		object_type::operator=(std::move(other));
		m_access = other.m_access; // XXX Choose weakest access rights
		m_id = other.m_id;
		return *this;
	}

	// Close process handle and invalidate process object
	// Mutates: from invalidate() 
	template <windows::access_rights T>
	void process<T>::close_process()
	{
		if (this->valid())
		{
			this->close_object();
			this->invalidate();
		}
	}

	// Close process handle
	// Mutates: from invalidate() 
	template <windows::access_rights T>
	process<T>::~process() { this->close_process(); }

	// Invalidate process id and handle
	// Mutates: m_id, m_handle
	template <windows::access_rights T>
	void process<T>::invalidate() { m_id = std::numeric_limits<id_type>::infinity(); }

} // end namespace distant