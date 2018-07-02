// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once
#include "../unsafe_process.hpp"

// Implementation header of distant::kernel_objects::process
#include <distant/sync/wait.hpp>

#include <distant/support/filesystem.hpp>
#include <distant/support/winapi/process.hpp>
#include <distant/support/winapi/debug.hpp>
#include <distant/support/winapi/wow64.hpp>
#include <boost/winapi/limits.hpp>

#include <boost/assert.hpp>

#define FORBID_INLINE __declspec(noinline)

namespace distant::kernel_objects
{
//protected:
	inline unsafe_handle unsafe_process::open(const std::size_t pid, process_rights access) noexcept
	{
		using flag_t = std::underlying_type_t<process_rights>;

#pragma warning(push)
#pragma warning(disable:4267)
		const auto result = boost::winapi::OpenProcess(static_cast<flag_t>(access), false, pid);
#pragma warning(pop)
		return unsafe_handle(result);
	}

	inline std::size_t unsafe_process::get_pid(const unsafe_handle& h) noexcept
	{
		const auto id = boost::winapi::GetProcessId(h.native_handle());
		return static_cast<std::size_t>(id);
	}

	inline void unsafe_process::kill()
	{
		BOOST_ASSERT_MSG(this->valid(), "[unsafe_process::kill] invalid process handle");

		const unsigned int exit_code = 0;
		boost::winapi::TerminateProcess(this->handle_.native_handle(), exit_code);
	}

	inline std::size_t unsafe_process::handle_count() const
	{
		BOOST_ASSERT_MSG(this->valid(), "[unsafe_process::handle_count] invalid process handle");

		boost::winapi::DWORD_ count = 0;
		if (!::GetProcessHandleCount(this->handle_.native_handle(), &count))
			throw windows_error("[unsafe_process::handle_count] GetProcessHandleCount failed");

		return static_cast<std::size_t>(count);
	}

	inline bool unsafe_process::is_active() const
	{
		if (!this->valid()) return false;

		const sync::wait wait_for;

		// Return immediately
		const auto result = wait_for(*this, 0);

		return result == sync::wait::state::timeout;
	}

	inline bool unsafe_process::is_32bit() const
	{
		BOOST_ASSERT_MSG(this->valid(), "[unsafe_process::is_32bit] invalid process handle");

		// Note: Using bool here on some systems can corrupt the stack since
		// sizeof(bool) != sizeof(BOOL).
		boost::winapi::BOOL_ result = false;
		boost::winapi::IsWow64Process(
			this->handle_.native_handle(),
			reinterpret_cast<boost::winapi::PBOOL_>(&result)
		);

		return result;
	}

	inline bool unsafe_process::is_64bit() const
	{
		return !this->is_32bit();
	}

	inline bool unsafe_process::is_being_debugged() const
	{
		BOOST_ASSERT_MSG(this->valid(), "[unsafe_process::is_being_debugged] invalid process handle");

		// If we are considering the current process, use IsDebuggerPresent.
		if (this->id() == GetCurrentProcessId())
			return IsDebuggerPresent();

		// Otherwise use CheckRemoteDebuggerPresent.
		boost::winapi::BOOL_ result = false;
		boost::winapi::CheckRemoteDebuggerPresent(this->handle_.native_handle(), &result);
		return result;
	}

	inline bool unsafe_process::is_zombie() const
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
		return handle_ != nullptr && GetProcessVersion(this->id()) == 0;
#pragma warning(pop)
	}

	inline std::wstring unsafe_process::name() const
	{
		return this->file_path().filename().wstring();
	}

	inline filesystem::path unsafe_process::file_path() const
	{
		BOOST_ASSERT_MSG(this->valid(), "[unsafe_process::file_path] invalid process handle");
		
		wchar_t buffer[boost::winapi::max_path];
		boost::winapi::DWORD_ max_path = boost::winapi::max_path;

		if (!boost::winapi::query_full_process_image_name(this->handle_.native_handle(), 0, buffer, &max_path))
			throw windows_error("[unsafe_process::file_path] query_full_process_image_name failed");

		return buffer;
	}

//public:
	inline bool unsafe_process::valid() const noexcept
	{
		return handle_.valid() && !this->is_zombie();
	}

	inline unsafe_process::operator bool() const noexcept
	{
		return this->valid();
	}

//=========================//
// Process ctors and dtor  //
//=========================//
	// Empty initialize process
	inline unsafe_process::unsafe_process() noexcept
		: handle_()
		, access_rights_()
	{}

	inline unsafe_process::unsafe_process(const std::size_t id, const process_rights access) noexcept
		: handle_(this->open(id, access))
		, access_rights_(access)
	{}

	inline unsafe_process::unsafe_process(unsafe_process&& other) noexcept
		: handle_(std::move(other.handle_))
		, access_rights_(other.access_rights_)	
	{}

	inline unsafe_process::unsafe_process(unsafe_handle&& h, const process_rights access) noexcept
		: handle_(std::move(h))
		, access_rights_(access)
	{}

	inline unsafe_process& unsafe_process::operator=(unsafe_process&& other) noexcept
	{
		handle_ = std::move(other.handle_);
		access_rights_ = other.access_rights_;
		return *this;
	}

	//free:
	inline bool operator ==(const unsafe_process& lhs, const unsafe_process& rhs) noexcept
	{
		return lhs.id() == rhs.id();
	}

	inline bool operator !=(const unsafe_process& lhs, const unsafe_process& rhs) noexcept
	{
		return !operator==(lhs, rhs);
	}

} // end namespace distant::kernel_objects::detail
