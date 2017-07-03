#pragma once

#include <cstddef>
#include <limits>

#include <Windows.h>

#include <distant\process\handle.h>

namespace distant {

	// Ideas: 
	//		- Process as a container
	//		- Return view of memory into process
	//		- Process iterators return remote memory
	class process
	{
	public:
		enum class access_rights
		{
			all_access = PROCESS_ALL_ACCESS,
			system_information = PROCESS_SET_INFORMATION,
			suspend_resume = PROCESS_SUSPEND_RESUME,
			terminate = PROCESS_TERMINATE,

			vm_operation = PROCESS_VM_OPERATION,
			vm_read = PROCESS_VM_READ,
			vm_write = PROCESS_VM_WRITE,

			query_limited_information = PROCESS_QUERY_LIMITED_INFORMATION,
			query_information = PROCESS_QUERY_INFORMATION,
		};

		invalid_handle_t invalid_handle;

	public:
		using id_type	  = std::size_t;
		using flag_type   = access_rights;
		using handle_type = HANDLE;

	public:
		// Static process members
		static process get_current() { return GetCurrentProcess(); }

	public:
		// Null initialize process
		process() : 
			m_id(std::numeric_limits<id_type>::infinity()), 
			m_flags(access_rights::all_access), 
			m_handle(invalid_handle)
		{}

		// Start process by id
		process(id_type id) : 
			m_id(std::numeric_limits<id_type>::infinity()),
			m_flags(access_rights::all_access) 
		{ m_handle = this->open(); }

		// Start process by id, with flags
		process(id_type id, flag_type flags) : 
			m_id(id), 
			m_flags(flags), 
			m_handle(invalid_handle)
		{ m_handle = this->open(); }

		process(handle_type handle) :
			m_id(GetProcessId(handle)),
			m_handle(handle)
		{}

		// Move other process handle into our possesion
		process(process&& other) : 
			m_id(other.get_id()), 
			m_flags(other.get_flags()), 
			m_handle(other.get_handle())
		{ other.invalidate(); }

		process(const process&) = delete;
		process& operator =(const process&) = delete;

		// Accesors for id and flags
		id_type get_id()	  const { return m_id; }
		flag_type get_flags() const { return m_flags; }

		// Check if the process handle is valid
		bool valid_handle()  const 
		{ 
			return !(m_handle == invalid_handle && 
					 m_id == std::numeric_limits<id_type>::infinity()); 
		}

		// Convert to handle_type
		operator const handle_type&()	const { return m_handle; }
		const handle_type& get_handle()	const { return m_handle; }

		// Close process handle
		// Mutates: from invalidate() 
		~process() { this->close_handle(); }

	private:
		// Close process handle and invalidate process object
		// Mutates: from invalidate() 
		void close_handle()
		{
			if (this->valid_handle())
				CloseHandle(m_handle);

			this->invalidate();
		}

		// Invalidate process id and handle
		// Mutates: m_id, m_handle
		void invalidate()
		{
			m_id = std::numeric_limits<id_type>::infinity();
			m_handle = invalid_handle;
		}

		// Open process
		// Mutates: m_handle
		handle_type open()
		{
			if (m_id != 0)
				return (m_handle = OpenProcess(static_cast<DWORD>(this->m_flags), false, m_id));

			return invalid_handle;
		}

	protected:
		id_type m_id;
		flag_type m_flags;
		handle_type m_handle;

	}; // end class process


} // end namespace distant