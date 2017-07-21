#pragma once
//#include <stdafx.h>

#include <distant\process.h>
#include <distant\windows\wait.h>

namespace distant {

	//===========================//
	// Static process functions //
	//===========================//
	template <flag_type access_t>
	process process::get_current()
	{
		process current(windows::handle(GetCurrentProcess()), flag_type::all_access);
		return current;
	}
	
	// Type-safe version of OpenProcess
	template <flag_type access_t>
	process::handle_type process::open(id_type id)
	{
		using flag_t = std::underlying_type_t<flag_type>;
		using handle_value = windows::handle::value_type;
	
		if (id != 0)
		{
			handle_value result = OpenProcess(static_cast<flag_t>(access_t), false, id);
			return windows::handle(result); // Returns windows::handle with result as value
		}

		return windows::invalid_handle;
	}

	template <flag_type access_t>
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
	template <flag_type access_t>
	bool process::valid()  const
	{
		return this->weakly_valid() &&
			get_id() != std::numeric_limits<id_type>::infinity();
	}

	// Check if we have permission perform the given action
	template <flag_type access_t>
	bool process::check_permission(process::flag_type access) const
	{
		return (m_access & access) == access;
	}

	template <flag_type access_t>
	bool process::is_running()
	{
		using windows::wait;
		if (!this->valid()) return false;

		wait wait_for;
		wait::state state = wait::state::abandoned;

		// Ensure we have the synchronize access_rights
		// This is required to call WaitForSingleObject
		if (this->check_permission(flag_type::synchronize))
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
	template <flag_type access_t>
	process::process() :
		object_type(), // Empty initialize synchro
		m_id(std::numeric_limits<id_type>::infinity()),
		m_access(flag_type::all_access)
	{}

	// Open process by id
	template <flag_type access_t>
	process::process(id_type id) :
		object_type(this->open(id, flag_type::all_access)),
		m_id(id),
		m_access(flag_type::all_access)
	{ this->update_gle(); }

	// Open process by id, with flags
	template <flag_type access_t>
	process::process(id_type id, flag_type flags) :
		object_type(this->open(id, flags)),
		m_id(id),
		m_access(flags)
	{ this->update_gle(); }

	template <flag_type access_t>
	process::process(handle_type&& handle, flag_type flags) :
		object_type(std::move(handle)), // steal handle
		m_access(flags)					// steal access flags
	{ m_id = get_pid(m_handle); }		// retrieve process id
										// This is done after initialization to ensure the operation
										// is performed after moving handle into our possession.

	template <flag_type access_t>
	process::process(process&& other) :
		securable(std::move(other)),
		m_id(std::move(other.m_id)),
		m_access(std::move(other.m_access))
	{}

	template <flag_type access_t>
	process& process::operator=(process&& other)
	{
		object_type::operator=(std::move(other));
		m_access = other.m_access;
		m_id = other.m_id;
		return *this;
	}

	// Close process handle and invalidate process object
	// Mutates: from invalidate() 
	template <flag_type access_t>
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
	template <flag_type access_t>
	process::~process() { this->close_process(); }

	// Invalidate process id and handle
	// Mutates: m_id, m_handle
	template <flag_type access_t>
	void process::invalidate() { m_id = std::numeric_limits<id_type>::infinity(); }

} // end namespace distant