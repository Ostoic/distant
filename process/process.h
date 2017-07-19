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

#include <distant\detail\attorney.h>

namespace distant {

	// Ideas: 
	//		- Process as a container
	//		- Return view of memory into process
	//		- Process iterators return remote memory
	class process : public windows::kernel::securable
	{
	public:
		enum class access_rights : int
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
		//===========================//
		// Static process functions //
		//===========================//
		// Get current process
		static process get_current();

		// Type-safe version of OpenProcess
		static handle_type open(id_type id, flag_type flags);

		static handle_type create();

		static id_type get_pid(const handle_type& h);

	public:
		//====================================//
		// Process objects should not be copy //
		// constructible, nor copy assignable //
		//====================================//
		//process(const process&) = delete;
		//process& operator =(const process&) = delete;

		//====================================//
		// Process id and access_rights flags //
		// accessors                          //
		//====================================//
		id_type get_id()	   const { return m_id; }
		flag_type get_access() const { return m_access; }

		// Check if the process handle is valid
		bool valid() const;

		// Check if we have permission perform the given action
		bool check_permission(access_rights access) const;

		// Mutates: gle
		bool is_running();

		// Return the virtual memory of this process
		memory::vm get_vm() const { return memory::vm(*this); }

		// Implicitly convertible to a vm
		operator memory::vm() const { return this->get_vm(); }

		// Idea: ostream << operator for flag, id, etc

		//=========================//
		// Process ctors and dtors //
		//=========================//
		// Empty initialize process
		process();

		// Open process by id
		process(id_type id);

		// Open process by id, with flags
		process(id_type id, flag_type flags);

		// Take handle and valid process access_rights associated with the handle
		process(handle_type&& handle, flag_type flags);

		// Take handle and valid process access_rights associated with the handle
		process(process&& other);

		// Close process handle
		// Mutates: from invalidate() 
		~process();

		process& operator =(process&& other);

		friend inline void swap(process& lhs, process& rhs) 
		{
			using std::swap;
			swap(lhs.m_id, rhs.m_id);
			swap(lhs.m_access, rhs.m_access);
		}

		friend bool operator ==(const process&, const process&);
		friend bool operator !=(const process&, const process&);

	private:
		// Close process handle and invalidate process object
		// Mutates: from invalidate() 
		void close_process();

		// Invalidate process id and handle
		// Mutates: m_id, m_handle
		void invalidate();

	protected:
		id_type m_id;
		flag_type m_access;

	}; // end class process

	// Define flag operators for use with process::access_rights
	DEFINE_ENUM_FLAG_OPERATORS(process::access_rights)

	inline bool operator ==(const process& lhs, const process& rhs)
	{
		return lhs.m_handle == rhs.m_handle &&
			   lhs.m_id     == rhs.m_id     &&
			   lhs.m_access  == rhs.m_access;
	}

	inline bool operator !=(const process& lhs, const process& rhs)
	{
		return !operator==(lhs, rhs);
	}

} // end namespace distant