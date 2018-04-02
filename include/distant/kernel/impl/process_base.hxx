#pragma once
#include <distant/kernel/process_base.hpp>

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

// Implementation header of distant::kernel::process
#include <distant/wait.hpp>

#include <distant/support/filesystem.hpp>
#include <distant/support/winapi/process.hpp>
#include <distant/support/winapi/debug.hpp>
#include <distant/support/winapi/wow64.hpp>

#include <boost/winapi/limits.hpp>

#define FORBID_INLINE __declspec(noinline)

namespace distant::kernel
{
	//protected:
	inline process_base::handle_type process_base::open(const std::size_t pid, access_rights_t access) noexcept
	{
		using flag_t = std::underlying_type_t<flag_type>;

#pragma warning(push)
#pragma warning(disable:4267)
		const auto result = OpenProcess(static_cast<flag_t>(access), false, pid);
#pragma warning(pop)
		return handle_type(result);
	}

	inline std::size_t process_base::get_pid(const handle_type& h) noexcept
	{
		const auto native_handle = expose::native_handle(h);
		const auto id = GetProcessId(native_handle);
		return static_cast<std::size_t>(id);
	}

	inline void process_base::throw_if_invalid(const char* message) const
	{
		if (!this->valid())
			throw std::system_error(last_error(), message);
	}

	inline void process_base::kill()
	{
		this->throw_if_invalid("[process_base::terminate] invalid process");

		const unsigned int exit_code = 0;
		const auto native_handle = expose::native_handle(handle_);

		TerminateProcess(native_handle, exit_code);
	}

	inline bool process_base::is_active() const
	{
		this->throw_if_invalid("[process_base::is_active] invalid process");

		const wait wait_for;

		// Return immediately
		const auto result = wait_for(*this, 0);
		//error_.get_last();

		return result == wait::state::timeout;
	}

	inline bool process_base::is_32bit() const
	{
		this->throw_if_invalid("[process_base::is_32bit] invalid process");

		// Note: Using bool here on some systems can corrupt the stack since
		// sizeof(bool) != sizeof(BOOL).
		boost::winapi::BOOL_ result = false;
		boost::winapi::IsWow64Process(handle_.native_handle(), reinterpret_cast<boost::winapi::PBOOL_>(&result));

		///*if (!*/))
		/*	error_.get_last();
		else
			error_.set_success();*/

		return result;
	}

	inline bool process_base::is_64bit() const
	{
		return !this->is_32bit();
	}

	inline bool process_base::is_being_debugged() const
	{
		// If we are considering the current process, use IsDebuggerPresent.
		if (this->id_ == GetCurrentProcessId())
			return IsDebuggerPresent();

		this->throw_if_invalid("[process_base::is_being_debugged] invalid process");

		// Otherwise use CheckRemoteDebuggerPresent.
		boost::winapi::BOOL_ result = false;
		CheckRemoteDebuggerPresent(handle_.native_handle(), &result);
		//if (!)
			//error_.get_last();
		//else
			//error_.set_success();

		return result;
	}

	inline bool process_base::is_zombie() const
	{
		/* Use of GetProcessVersion:
		** I ran into a bug where OpenProcess would return a "valid" handle that did not refer to any process.
		** It was apparent that no such process existed because under task manager/procmon, no such id was
		** listed. I noticed that GetProcessVersion would perform some sort of integrity check on the process
		** id, which seemed to be consistent with processes on the manager. Indeed, the process handles I was
		** getting coincided with those from using the standard ToolHelp functions.
		*/

#pragma warning(push)
#pragma warning(disable:4267)
		return this->get_handle() != nullptr && GetProcessVersion(id_) == 0;
#pragma warning(pop)
	}

	inline std::wstring process_base::filename() const
	{
		return this->file_path().filename().wstring();
	}

	inline filesystem::path process_base::file_path() const
	{
		this->throw_if_invalid("[process_base::file_path] invalid process");

		const auto native_handle = expose::native_handle(handle_);

		wchar_t pathBuffer[boost::winapi::MAX_PATH_];
		boost::winapi::DWORD_ max_path = boost::winapi::MAX_PATH_;

		if (!boost::winapi::query_full_process_image_name(native_handle, 0, pathBuffer, &max_path))
			throw std::system_error(last_error(), "[process_base::file_path] query_full_process_image_name failed");
		//error_.set_success();

		return {pathBuffer};
	}

	//public:
	inline bool process_base::valid() const noexcept
	{
		return
			base_type::valid() &&
			!this->is_zombie() &&
			id_ != std::numeric_limits<std::size_t>::infinity();
	}

	//=========================//
	// Process ctors and dtor  //
	//=========================//

	// Note: We consider forbidding the compiler from inlining the ctors to reduce code bloat
	// Upon compiling in release mode with the Visual Studio 2017 compiler,
	// the process_base::{ctor} would get inlined, but the compiler would create copies
	// of the same ctor kernel::process code. When we forbid inlining for the process_base,
	// we observe only calls to process_base::ctor, meaning the kernel::process ctors are 
	// completely optimized out.

	// Empty initialize process
	inline process_base::process_base() noexcept
		: base_type()
		  , id_(std::numeric_limits<std::size_t>::infinity())
		  , access_rights_()
	{}

	inline process_base::process_base(const std::size_t id, const access_rights_t access) noexcept
		: base_type(this->open(id, access))
		  , id_(id)
		  , access_rights_(access)
	{}

	inline process_base::process_base(process_base&& other) noexcept
		: base_type(std::move(other))
		  , id_(std::move(other.id_))
		  , access_rights_(std::move(other.access_rights_))
	{}

	// XXX Choose weakest access rights or produce error about incompatible access rights

	inline process_base::process_base(handle<process_base>&& h, const access_rights_t access) noexcept
		: object(std::move(reinterpret_cast<handle<object>&>(h)))
		  , id_(GetProcessId(handle_.native_handle()))
		  , access_rights_(access)
	{}

	inline process_base& process_base::operator=(process_base&& other) noexcept
	{
		this->access_rights_ = other.access_rights_;
		this->id_ = other.id_;
		base_type::operator=(std::move(other));
		return *this;
	}

	//free:
	inline bool operator ==(const process_base& lhs, const process_base& rhs) noexcept
	{
		return /*lhs.handle_ == rhs.handle_ &&*/
			lhs.id_ == rhs.id_;
		//lhs.m_access == rhs.m_access;
	}

	inline bool operator !=(const process_base& lhs, const process_base& rhs) noexcept
	{
		return !operator==(lhs, rhs);
	}

	inline process_base current_process() noexcept
	{
		return process_base{
			handle<process_base>{GetCurrentProcess(), access_rights::handle::close_protected}, access_rights::process::all_access
		};
	}
} // end namespace distant::kernel::detail
