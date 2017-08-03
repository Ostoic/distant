#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <cstddef>
#include <limits>

#include <string>
#include <Windows.h>

#include <distant\windows\kernel\object.hpp>
#include <distant\windows\access_rights.hpp>
#include <distant\windows\handle.hpp>

#include <distant\type_traits.hpp>

//#include <distant\memory\vm.h>

namespace distant {
namespace windows {
namespace kernel  {
	
	// Windows process object extending a kernel securable object.
	// This is the main class to use when querying process information in 
	// this library.
	template <access_rights::process access_t>
	class process : public windows::kernel::object
	{
	public:
		// Object type information
		using base_type   = windows::kernel::object;

		using error_type  = typename object_traits<process>::error_type;
		using handle_type = typename object_traits<process>::handle_type;

		using exit_code_type = std::size_t;

		// Process type information
		using id_type = std::size_t;
		using flag_type = access_rights::process;

	public: 
		//===========================//
		// Static process functions  //
		//===========================//
		// Get current process
		static process get_current();

		// Type-safe version of OpenProcess
		static handle_type open(id_type);

		static handle_type create();

		static void terminate(const handle_type&);

		static id_type get_pid(const handle_type&);

		static std::size_t get_handle_count(const handle_type&);

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

		const windows::handle<process>& get_handle() const { return object::get_handle<process>(); }

		// Check if the process handle is valid
		bool valid() const;

		void terminate() const;

		//std::string name() const;

		// Function used to to check if we have permission perform the given action
		inline static constexpr bool check_permission(flag_type access);

		// Mutates: gle
		bool is_running() const;

		std::size_t get_handle_count() const;
		//inline const handle_type& get_handle() const { return m_handle; }

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
		explicit process(id_type id);

		// Take handle and valid process access_rights associated with the handle
		process(handle_type&& handle);

		// Take handle and valid process access_rights associated with the handle
		process(process&& other);

		// Close process handle
		// Mutates: from invalidate() 
		~process();

		process& operator =(process&& other);

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

	template <access_rights::process access_t>
	inline bool operator ==(const process<access_t>& lhs, const process<access_t>& rhs)
	{
		return lhs.m_handle == rhs.m_handle &&
			   lhs.m_id     == rhs.m_id     &&
			   lhs.m_access  == rhs.m_access;
	}

	template <access_rights::process access_t>
	inline bool operator !=(const process<access_t>& lhs, const process<access_t>& rhs)
	{
		return !operator==(lhs, rhs);
	}

} // end namespace kernel
} // end namespace windows
} // end namespace distant

#include <distant\windows\kernel\detail\process.inl>