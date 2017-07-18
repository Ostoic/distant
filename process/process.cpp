#pragma once
#include <stdafx.h>

#include <distant\process\process.h>
#include <distant\windows\wait.h>

#include <memory>

namespace distant {

	//===========================//
	// Static process functions //
	//===========================//
	process process::get_current()
	{
		process current(windows::handle(GetCurrentProcess()), access_rights::all_access);
		return current;
	}
	
	// Type-safe version of OpenProcess
	process::handle_type process::open(id_type id, flag_type access)
	{
		using flag_t = std::underlying_type_t<flag_type>;
		using handle_value = windows::handle::value_type;
	
		if (id != 0)
		{
			handle_value result = OpenProcess(static_cast<flag_t>(access), false, id);
			return windows::handle(result); // Returns windows::handle with result as value
		}

		return windows::invalid_handle;
	}

	process::id_type process::get_pid(const handle_type& h)
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
	bool process::valid()  const
	{
		return this->weakly_valid() &&
			get_id() != std::numeric_limits<id_type>::infinity();
	}

	// Check if we have permission perform the given action
	bool process::check_permission(process::access_rights access) const
	{
		return (m_access & access) == access;
	}

	bool process::is_running()
	{
		using windows::wait;
		if (!this->valid()) return false;

		wait wait_for;
		wait::state state;

		// Ensure we have the synchronize access_rights
		// This is required to call WaitForSingleObject
		if (this->check_permission(access_rights::synchronize))
		{
			state = wait_for(*this);
			this->update_gle(wait_for);
		}

		return state == wait::state::timeout;
	}

	//=========================//
	// Process ctors and dtor  //
	//=========================//
	// Empty initialize process
	process::process() :
		object_type(), // Empty initialize synchro
		m_id(std::numeric_limits<id_type>::infinity()),
		m_access(access_rights::all_access)
	{}

	// Open process by id
	process::process(id_type id) :
		object_type(this->open(id, access_rights::all_access)),
		m_id(id),
		m_access(access_rights::all_access)
	{ this->update_gle(); }

	// Open process by id, with flags
	process::process(id_type id, flag_type flags) :
		object_type(this->open(id, flags)),
		m_id(id),
		m_access(flags)
	{ this->update_gle(); }

	process::process(handle_type&& handle, flag_type flags) :
		object_type(std::move(handle)), // steal handle
		m_access(flags)					// steal access flags
	{ m_id = get_pid(m_handle); }		// retrieve process id
										// This is done after initialization to ensure the operation
										// is performed after moving handle into our possession.

	process::process(process&& other) :
		securable(std::move(other)),
		m_id(std::move(other.m_id)),
		m_access(std::move(other.m_access))
	{}

	process& process::operator=(process&& other)
	{
		using std::swap;
		swap(*this, other);
		return *this;
	}

	// Close process handle and invalidate process object
	// Mutates: from invalidate() 
	void process::close_process()
	{
		if (this->valid())
		{
			this->close_object();
			this->invalidate();
		}
	}

	// Close process handle
	// Mutates: from invalidate() 
	process::~process() { this->close_process(); }

	// Invalidate process id and handle
	// Mutates: m_id, m_handle
	void process::invalidate() { m_id = std::numeric_limits<id_type>::infinity(); }

} // end namespace distant