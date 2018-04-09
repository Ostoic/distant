// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once
#include <distant/kernel_objects/process_base.hpp>

// Implementation header of distant::kernel_objects::process
#include <distant/synch/wait.hpp>

#include <distant/support/filesystem.hpp>
#include <distant/support/winapi/process.hpp>
#include <distant/support/winapi/debug.hpp>
#include <distant/support/winapi/wow64.hpp>

#include <boost/winapi/limits.hpp>

#define FORBID_INLINE __declspec(noinline)

namespace distant::kernel_objects
{
	//protected:
	inline process_base::handle_type process_base::open(const std::size_t pid, access_rights_t access) noexcept
	{
		using flag_t = std::underlying_type_t<flag_type>;

#pragma warning(push)
#pragma warning(disable:4267)
		const auto result = boost::winapi::OpenProcess(static_cast<flag_t>(access), false, pid);
#pragma warning(pop)
		return handle_type(result);
	}

	inline std::size_t process_base::get_pid(const handle_type& h) noexcept
	{
		const auto id = GetProcessId(h.native_handle());
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
		boost::winapi::TerminateProcess(this->handle_.native_handle(), exit_code);
	}

	inline std::size_t process_base::handle_count() const
	{
		this->throw_if_invalid("[process_base::handle_count] invalid process");

		boost::winapi::DWORD_ count = 0;
		if (!::GetProcessHandleCount(this->handle_.native_handle(), &count))
			throw std::system_error(error::last_error(), "[process_base::handle_count] GetProcessHandleCount failed");

		return static_cast<std::size_t>(count);
	}

	inline bool process_base::is_active() const
	{
		if (!this->valid()) return false;

		const synch::wait wait_for;

		// Return immediately
		const auto result = wait_for(*this, 0);

		return result == synch::wait::state::timeout;
	}

	inline bool process_base::is_32bit() const
	{
		this->throw_if_invalid("[process_base::is_32bit] invalid process");

		// Note: Using bool here on some systems can corrupt the stack since
		// sizeof(bool) != sizeof(BOOL).
		boost::winapi::BOOL_ result = false;
		boost::winapi::IsWow64Process(this->handle_.native_handle(), reinterpret_cast<boost::winapi::PBOOL_>(&result));

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
		boost::winapi::CheckRemoteDebuggerPresent(this->handle_.native_handle(), &result);
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
		** Note: I didn't realize zombie processes exist on Windows in this form.
		*/

#pragma warning(push)
#pragma warning(disable:4267)
		return this->handle() != nullptr && GetProcessVersion(this->id_) == 0;
#pragma warning(pop)
	}

	inline std::wstring process_base::name() const
	{
		return this->file_path().filename().wstring();
	}

	inline filesystem::path process_base::file_path() const
	{
		this->throw_if_invalid("[process_base::file_path] invalid process");
		
		wchar_t buffer[boost::winapi::max_path];
		boost::winapi::DWORD_ max_path = boost::winapi::max_path;

		if (!boost::winapi::query_full_process_image_name(this->handle_.native_handle(), 0, buffer, &max_path))
			throw std::system_error(last_error(), "[process_base::file_path] query_full_process_image_name failed");

		return buffer;
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
		  , id_(other.id_)
		  , access_rights_(other.access_rights_)	
	{}

	inline process_base::process_base(distant::handle<process_base>&& h, const access_rights_t access) noexcept
		: kernel_object(std::move(reinterpret_cast<distant::handle<kernel_object>&>(h)))
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

} // end namespace distant::kernel_objects::detail
