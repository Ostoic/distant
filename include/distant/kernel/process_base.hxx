#pragma once
#include <distant\kernel\process_base.hpp>

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

// Implementation header of distant::kernel::process
#include <distant\wait.hpp>

#include <distant\support\filesystem.hpp>
#include <distant\support\winapi\process.hpp>

#include <boost\winapi\process.hpp>
#include <boost\winapi\error_codes.hpp>
#include <boost\winapi\limits.hpp>
#include <boost\winapi\get_current_process_id.hpp>

#define FORBID_INLINE __declspec(noinline)

namespace distant::kernel {

//protected:
	inline typename process_base::handle_type process_base::open(pid_type pid, access_rights_t access) noexcept
	{
		using flag_t = std::underlying_type_t<flag_type>;

		const auto result = boost::winapi::OpenProcess(static_cast<flag_t>(access), false, pid);
		return handle_type(result); 
	}

	inline typename process_base::pid_type process_base::get_pid(const handle_type& h) noexcept
	{
		const auto native_handle = expose::native_handle(h);
		const auto id = boost::winapi::GetProcessId(native_handle);
		return static_cast<pid_type>(id);
	}


	inline void process_base::terminate() const
	{
		if (!this->valid())
		{
			m_last_error.set(boost::winapi::ERROR_INVALID_HANDLE_);
			return;
			//throw std::system_error(m_last_error, "Invalid process handle in process::terminate.");
		}

		const unsigned int exit_code = 0;
		const auto native_handle = expose::native_handle(m_handle);

		if (!boost::winapi::TerminateProcess(native_handle, exit_code))
			m_last_error.update();
		else
			m_last_error.set_success();

		return;
	}

	inline bool process_base::is_active() const
	{
		if (!this->valid())
		{
			m_last_error.set(boost::winapi::ERROR_INVALID_HANDLE_);
			return false;
			//throw std::system_error(m_last_error, "Invalid process handle in process::is_active.");
		}

		wait wait_for;

		// Return immediately
		const auto result = wait_for(*this, 0);
		m_last_error.update();

		return result == wait::state::timeout;
	}

	inline std::string process_base::filename() const
	{
		return this->file_path().filename().string();
	}

	inline distant::filesystem::path process_base::file_path() const
	{
		if (!this->valid())
		{
			m_last_error.set(boost::winapi::ERROR_INVALID_HANDLE_);
			throw std::system_error(m_last_error, "process::is_active failed.");
		}

		const auto native_handle = expose::native_handle(m_handle);

		char out_path[boost::winapi::MAX_PATH_] = "";
		boost::winapi::DWORD_ max_path = boost::winapi::MAX_PATH_;

		// 0 indicates: The name should use the Win32 path format.
		if (!boost::winapi::query_full_process_image_name(native_handle, 0, out_path, &max_path))
		{
			m_last_error.update();
			throw std::system_error(m_last_error, "process::is_active failed.");
		}
		else
			m_last_error.set_success();

		return { out_path };
	}

//public:
	inline bool process_base::valid() const noexcept
	{
		return base_type::valid() &&
			id() != std::numeric_limits<pid_type>::infinity();
	}

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
	process_base::process_base() noexcept
		: base_type()
		, m_id(std::numeric_limits<pid_type>::infinity())
		, m_access_rights() {}

	FORBID_INLINE 
	process_base::process_base(pid_type id, access_rights_t access) noexcept
		: base_type(this->open(id, access))
		, m_id(id)
		, m_access_rights(access)
	{ 
		if (!this->valid())
			m_last_error.update();
	}

	FORBID_INLINE 
	process_base::process_base(process_base&& other) noexcept
		: base_type(std::move(other))
		, m_id(std::move(other.m_id))
		, m_access_rights(std::move(other.m_access_rights)) {} 
	// XXX Choose weakest access rights or produce error about incompatible access rights

	FORBID_INLINE 
	process_base& process_base::operator=(process_base&& other) noexcept
	{
		base_type::operator=(std::move(other));
		m_access_rights = other.m_access_rights;
		m_id = other.m_id;
		return *this;
	}

//free:
	FORBID_INLINE bool operator ==(const process_base& lhs, const process_base& rhs) noexcept
	{
		return /*lhs.m_handle == rhs.m_handle &&*/
			lhs.m_id == rhs.m_id;
		//lhs.m_access == rhs.m_access;
	}

	FORBID_INLINE bool operator !=(const process_base& lhs, const process_base& rhs) noexcept
	{
		return !operator==(lhs, rhs);
	}

} // end namespace distant::kernel::detail
