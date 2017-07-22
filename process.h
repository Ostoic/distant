#pragma once

#include <cstddef>
#include <limits>

#include <Windows.h>

#include <distant\windows\kernel\securable.h>
#include <distant\windows\access_rights.h>
#include <distant\windows\handle.h>
#include <distant\windows\wait.h>

#include <distant\memory\address.h>
//#include <distant\memory\vm.h>

#include <distant\detail\attorney.h>

namespace distant {


	// Ideas: 
	//		- Process as a container
	//		- Return view of memory into process
	//		- Process iterators return remote memory
	template <windows::access_rights access_t>
	class process : public windows::kernel::securable
	{
	public:
		// Object type information
		using object_type = windows::kernel::securable;
		using handle_type = object_type::handle_type;
		using error_type  = object_type::error_type;

		// Process type information
		using id_type = std::size_t;
		using flag_type = windows::access_rights;

	public: 
		//===========================//
		// Static process functions //
		//===========================//
		// Get current process
		static process get_current();

		// Type-safe version of OpenProcess
		static handle_type open(id_type id);

		static handle_type create();

		static id_type get_pid(const handle_type& h);

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
		id_type get_id()	   const { return m_id; }
		flag_type get_access() const { return m_access; }

		// Check if the process handle is valid
		bool valid() const;

		// Check if we have permission perform the given action
		constexpr bool check_permission(flag_type access) const;

		template <windows::access_rights other_t>
		using check_permission_t = std::conditional_t<(other_t & access_t) == access_t, std::true_type, std::false_type>;

		//template <windows::access_rights other_t>
		//using check_permission_v = check_permission_t<other_t>::value;

		// Mutates: gle
		bool is_running() const;

		// Return the virtual memory of this process
		//memory::vm get_vm() const { return memory::vm(*this); }

		// Implicitly convertible to a vm
		//operator memory::vm() const { return this->get_vm(); }

		// Idea: ostream << operator for flag, id, etc

		//=========================//
		// Process ctors and dtors //
		//=========================//
		// Empty initialize process
		constexpr process();

		// Open process by id
		process(id_type id);

		// Open process by id, with flags
		//process(id_type id);

		// Take handle and valid process access_rights associated with the handle
		process(handle_type&& handle);

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

	template <windows::access_rights access_t>
	inline bool operator ==(const process<access_t>& lhs, const process<access_t>& rhs)
	{
		return lhs.m_handle == rhs.m_handle &&
			   lhs.m_id     == rhs.m_id     &&
			   lhs.m_access  == rhs.m_access;
	}

	template <windows::access_rights access_t>
	inline bool operator !=(const process<access_t>& lhs, const process<access_t>& rhs)
	{
		return !operator==(lhs, rhs);
	}

} // end namespace distant

#include <distant\process\process.inl>