#pragma once

#include <cstddef>
#include <limits>
#include <bitset>

#include <Windows.h>

#include <distant\windows\synchro.h>
#include <distant\windows\handle.h>

#include <distant\memory\address.h>
#include <distant\memory\vm.h>

namespace distant {

	// Ideas: 
	//		- Process as a container
	//		- Return view of memory into process
	//		- Process iterators return remote memory
	class process : public windows::synchro
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
		using synchro_type = windows::synchro;
		using handle_type  = synchro_type::handle_type;
		using error_type   = synchro_type::error_type;

		// Process type information
		using id_type = std::size_t;
		using flag_type = access_rights;

	public: // Static process members

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
		id_type get_id()	  const { return m_id; }
		flag_type get_flags() const { return m_flags; }

		// Check if the process handle is valid
		bool valid_process()  const 
		{ 
			return !(this->valid_handle() && 
					 m_id == std::numeric_limits<id_type>::infinity()); 
		}

		// Check if we have permission perform the given action
		bool check_permission(access_rights access) const;

		// Mutates: gle
		bool is_running() 
		{
			if (!this->valid_process()) return false;

			DWORD result;

			// Ensure we have the synchronize access_rights
			// This is required to call WaitForSingleObject
			if (this->check_permission(access_rights::synchronize))
			{
				result = WaitForSingleObject(m_handle, 0);
				this->update_gle();
			}

			//ret = WaitForSingleObject(process, 0);
			//return ret == WAIT_TIMEOUT;
			return result == WAIT_TIMEOUT;
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
			synchro(), // Empty initialize synchro
			m_id(std::numeric_limits<id_type>::infinity()),
			m_flags(access_rights::all_access)
		{}

		// Open process by id
		process(id_type id) :
			synchro(this->open(id, access_rights::all_access)),
			m_id(id),
			m_flags(access_rights::all_access)
		{}

		// Open process by id, with flags
		process(id_type id, flag_type flags) :
			synchro(this->open(id, flags)),
			m_id(id),
			m_flags(flags)
		{}

		// Take handle and valid process access_rights associated with the handle
		process(handle_type&& handle, flag_type flags) :
			synchro(std::move(handle)),
			m_id(GetProcessId(handle)),
			m_flags(flags)
		{}

		// Move other process handle into our possesion
		process(process&& other) :
			synchro(static_cast<synchro>(other)),
			m_id(other.get_id()),
			m_flags(other.get_flags())
		{ other.invalidate(); }

		// Close process handle
		// Mutates: from invalidate() 
		~process() { this->close_handle(); }

		friend bool operator ==(const process&, const process&);
		friend bool operator !=(const process&, const process&);

	private:
		// Close process handle and invalidate process object
		// Mutates: from invalidate() 
		void close_process()
		{
			if (this->valid_process())
			{
				this->close_handle();
				this->invalidate();
			}
		}

		// Invalidate process id and handle
		// Mutates: m_id, m_handle
		void invalidate()
		{
			m_id = std::numeric_limits<id_type>::infinity();
		}

		// Open process
		// Mutates: m_handle
		handle_type open(id_type id, flag_type flags)
		{
			using T = std::underlying_type_t<flag_type>;

			if (id != 0)
			{
				auto result = OpenProcess(static_cast<T>(flags), false, id);
				this->update_gle();
				return result;
			}

			return invalid_handle;
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