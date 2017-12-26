#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

// Implementation header of distant::kernel::process
#include <distant\kernel\detail\process_base.hpp>
#include <distant\wait.hpp>

#include <distant\support\filesystem.hpp>

#include <boost\winapi\process.hpp>
#include <boost\winapi\error_codes.hpp>
#include <boost\winapi\limits.hpp>

#define FORBID_INLINE __declspec(noinline)

namespace distant::kernel::detail {

//protected:
	inline typename process_base::handle_type process_base::open(pid_type pid, access_rights_t access)
	{
		namespace winapi = boost::winapi;
		using flag_t = std::underlying_type_t<flag_type>;

		const auto result = winapi::OpenProcess(static_cast<flag_t>(access), false, pid);
		return handle_type(result); // Returns handle with result as value
	}

	inline typename process_base::pid_type process_base::get_pid(const handle_type& h)
	{
		const auto native_handle = expose::native_handle(h);
		const auto id = ::GetProcessId(native_handle);
		return static_cast<pid_type>(id);
	}


	inline void process_base::terminate() const
	{
		namespace winapi = boost::winapi;

		if (!this->valid())
		{
			this->set_last_error(winapi::ERROR_INVALID_HANDLE_);
			return;
		}

		const unsigned int exit_code = 0;
		auto native_handle = expose::native_handle(m_handle);

		if (!winapi::TerminateProcess(native_handle, exit_code))
			this->update_gle();
		else
			this->set_success();
		return;
	}

	inline bool process_base::is_active() const
	{
		namespace winapi = boost::winapi;
		if (!this->valid())
		{
			this->set_last_error(winapi::ERROR_INVALID_HANDLE_);
			return false;
		}

		wait wait_for;

		// Return immediately
		const auto result = wait_for(*this, 0);
		this->update_gle(wait_for);

		return result == wait::state::timeout;
	}

	inline std::string process_base::filename() const
	{
		return this->file_path().filename().string();
	}

	inline distant::filesystem::path process_base::file_path() const
	{
		namespace winapi = boost::winapi;

		if (!this->valid())
			//throw std::invalid_argument(this->format_gle());
		{
			this->set_last_error(winapi::ERROR_INVALID_HANDLE_);
			return "";
		}

		const auto native_handle = expose::native_handle(m_handle);

		char out_path[winapi::MAX_PATH_] = "";
		DWORD max_path = winapi::MAX_PATH_;

		// 0 indicates: The name should use the Win32 path format.
		if (!::QueryFullProcessImageNameA(native_handle, 0, out_path, &max_path))
			this->update_gle();
		else
			this->set_success();

		return { out_path };
	}

//public:
	//====================================//
	// Process id and access_rights flags //
	// accessors                          //
	//====================================//
	// Check if the process handle is valid
	inline bool process_base::valid()  const
	{
		return base_type::valid() &&
			pid() != std::numeric_limits<pid_type>::infinity();
	}

	/*inline auto process_base::memory_status() const
	{
		return memory_status_t{ *this };
	}*/

	/*inline const handle<process_base>&
		process_base::get_handle() const
	{
		return object::get_handle<process_base>();
	}*/

	//=========================//
	// Process ctors and dtor  //
	//=========================//

	// Note: We forbid the compiler from inlining the ctors to reduce code bloat
	// Upon compiling in release mode with the Visual Studio 2017 compiler,
	// the process_base::ctor would get inlined, but the compiler would create copies
	// of the same ctor kernel::process code. When we forbid inlining for the process_base,
	// we observe only calls to process_base::ctor, meaning the kernel::process ctors are 
	// completely optimized out.

	// Empty initialize process
	FORBID_INLINE 
	constexpr process_base::process_base()
		: base_type()
		, m_pid(std::numeric_limits<pid_type>::infinity())
		, m_access() {}

	FORBID_INLINE 
	process_base::process_base(pid_type id, access_rights_t access)
		: base_type(this->open(id, access))
		, m_pid(id)
		, m_access(access)
	{ update_gle(); }

	// Take possession of process handle. It is ensured to be a convertible process handle
	// due to encoded type in handle.
	FORBID_INLINE 
	process_base::process_base(handle_type&& handle, access_rights_t access)
		: base_type(std::move(handle))	// steal handle
		, m_access(access)				
	{ m_pid = get_pid(get_handle<process_base>()); }	// retrieve process id
				// This is done after initialization to ensure the operation
				// is performed after moving handle into our possession.

	FORBID_INLINE 
	process_base::process_base(process_base&& other) 
		: base_type(std::move(other))
		, m_pid(std::move(other.m_pid))
		, m_access(std::move(other.m_access)) {} 
	// XXX Choose weakest access rights or produce error about incompatible access rights

	FORBID_INLINE 
	process_base& process_base::operator=(process_base&& other)
	{
		base_type::operator=(std::move(other)); // This should invalide other
		m_access = other.m_access;
		m_pid = other.m_pid;
		return *this;
	}
	
	process_base::operator bool() const
	{
		return this->valid();
	}

//free:
	FORBID_INLINE bool operator ==(const process_base& lhs, const process_base& rhs)
	{
		return /*lhs.m_handle == rhs.m_handle &&*/
			lhs.m_pid == rhs.m_pid;
		//lhs.m_access == rhs.m_access;
	}

	FORBID_INLINE bool operator !=(const process_base& lhs, const process_base& rhs)
	{
		return !operator==(lhs, rhs);
	}

} // end namespace distant::kernel::detail
