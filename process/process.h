#pragma once

#include <cstddef>
#include <limits>
#include <bitset>

#include <Windows.h>

#include <distant\windows\kernel\securable.h>
#include <distant\windows\handle.h>
#include <distant\windows\wait.h>

#include <distant\memory\address.h>
#include <distant\memory\vm.h>

namespace distant {

	// Ideas: 
	//		- Process as a container
	//		- Return view of memory into process
	//		- Process iterators return remote memory
	class process : public windows::kernel::securable
	{
	public:
		enum class access_rights
		{
			all_access = PROCESS_ALL_ACCESS,

			set_information = PROCESS_SET_INFORMATION,
			set_quota = PROCESS_SET_QUOTA,

			vm_operation = PROCESS_VM_OPERATION,
			vm_read = PROCESS_VM_READ,
			vm_write = PROCESS_VM_WRITE,

			create_process = PROCESS_CREATE_PROCESS,
			create_thread = PROCESS_CREATE_THREAD,
			dup_handle = PROCESS_DUP_HANDLE,

			suspend_resume = PROCESS_SUSPEND_RESUME,
			terminate = PROCESS_TERMINATE,

			query_limited_information = PROCESS_QUERY_LIMITED_INFORMATION,
			query_information = PROCESS_QUERY_INFORMATION,

			synchronize = SYNCHRONIZE,
		};

	public:
		// Synchro type information
		using object_type = windows::kernel::securable;
		using handle_type = object_type::handle_type;
		using error_type  = object_type::error_type;

		// Process type information
		using id_type = std::size_t;
		using flag_type = access_rights;

	public: 
		// Static process members
		// Get current process
		static process get_current() 
		{ 
			//std::shared_ptr<int>;
			process current(GetCurrentProcess(), access_rights::all_access);
			return current;
		}

	public:
		//====================================//
		// Process objects should not be copy //
		// constructible, nor copy assignable //
		//====================================//
		process(const process&) = delete;
		process& operator =(const process&) = delete;

		//====================================//
		// Process id and access_rights flags //
		// accessors                          //
		//====================================//
		inline id_type get_id()		 const { return m_id; }
		inline flag_type get_flags() const { return m_flags; }

		// Check if the process handle is valid
		bool valid_process()  const 
		{ 
			return !(this->weakly_valid() && 
					 get_id() == std::numeric_limits<id_type>::infinity()); 
		}

		// Check if we have permission perform the given action
		bool check_permission(access_rights access) const;

		// Mutates: gle
		bool is_running() 
		{
			using namespace windows;
			if (!this->valid_process()) return false;

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

		// Return the virtual memory of this process
		memory::vm get_vm() const { return memory::vm(*this); }

		// Implicitly convertible to a vm
		operator memory::vm() const { return this->get_vm(); }

		// Idea: ostream << operator for flag, id, etc

		//=========================//
		// Process ctors and dtors //
		//=========================//
		// Empty initialize process
		process() :
			object_type(), // Empty initialize synchro
			m_id(std::numeric_limits<id_type>::infinity()),
			m_flags(access_rights::all_access)
		{}

		// Open process by id
		process(id_type id) :
			object_type(this->open(id, access_rights::all_access)),
			m_id(id),
			m_flags(access_rights::all_access)
		{}

		// Open process by id, with flags
		process(id_type id, flag_type flags) :
			object_type(this->open(id, flags)),
			m_id(id),
			m_flags(flags)
		{}

		// Take handle and valid process access_rights associated with the handle
		process(handle_type&& handle, flag_type flags) :
			object_type(std::move(handle)),
			m_id(GetProcessId(handle)),
			m_flags(flags)
		{}

		// Move other process handle into our possesion
		process(process&& other) :
			object_type(std::move(other)),
			m_id(other.get_id()),
			m_flags(other.get_flags())
		{}

		// Close process handle
		// Mutates: from invalidate() 
		~process() { this->close_process(); }

		friend bool operator ==(const process&, const process&);
		friend bool operator !=(const process&, const process&);

	private:
		// Close process handle and invalidate process object
		// Mutates: from invalidate() 
		void close_process()
		{
			if (this->valid_process())
			{
				this->close_object();
				this->invalidate();
			}
		}

		// Invalidate process id and handle
		// Mutates: m_id, m_handle
		void invalidate() { m_id = std::numeric_limits<id_type>::infinity(); }

		// Type-safe version of OpenProcess
		// Mutates: m_handle
		handle_type open(id_type id, flag_type flags)
		{
			using flag_t = std::underlying_type_t<flag_type>;
			using handle_value = handle::value_type;

			if (id != 0)
			{
				handle_value result = OpenProcess(static_cast<flag_t>(flags), false, id);
				this->update_gle();
				return result; // Returns windows::handle with result as value
			}

			
			
			return windows::invalid_handle;
		}

		id_type get_pid(handle_type h)
		{
			h.get_value();
		}

	protected:
		id_type m_id;
		flag_type m_flags;

	}; // end class process

	// Define flag operators for use with process::access_rights
	DEFINE_ENUM_FLAG_OPERATORS(process::access_rights)

	// Check if we have permission perform the given action
	bool process::check_permission(process::access_rights access) const
	{
		return (m_flags & access) == access;
	}

	inline bool operator ==(const process& lhs, const process& rhs)
	{
		return lhs.m_handle == rhs.m_handle &&
			   lhs.m_id     == rhs.m_id     &&
			   lhs.m_flags  == rhs.m_flags;
	}

	inline bool operator !=(const process& lhs, const process& rhs)
	{
		return !operator==(lhs, rhs);
	}

} // end namespace distant