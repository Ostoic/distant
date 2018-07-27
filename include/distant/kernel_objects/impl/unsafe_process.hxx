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

#include <distant/error/winapi_error.hpp>

#include <boost/assert.hpp>

#define FORBID_INLINE __declspec(noinline)

namespace distant::kernel_objects
{
//public:
	inline void unsafe_process::kill()
	{
		BOOST_ASSERT_MSG(this->valid(), "[unsafe_process::kill] invalid process handle");

		const unsigned int exit_code = 0;
		boost::winapi::TerminateProcess(this->handle_.native_handle(), exit_code);
	}

	inline void unsafe_process::join()
	{
		if (!this->joinable())
			throw std::invalid_argument("[unsafe_process::join] Join called on unjoinable process");

		if (this->id() == unsafe_process::id_t(::GetCurrentProcessId()))
			throw std::invalid_argument("[unsafe_process::join] Join on current process, deadlock would occur");

		namespace winapi = boost::winapi;
		if (sync::wait(*this) == sync::state::failed)
			throw winapi_error("[unsafe_process::join] WaitForSingleObjectEx failed");

		winapi::DWORD_ exit_code;
		if (GetExitCodeThread(
				handle_.native_handle(),
				&exit_code)
			== 0
		)
			throw winapi_error("[unsafe_process::join] GetExitCodeThread failed");

		this->detach_unchecked();
	}

	inline void unsafe_process::detach()
	{
		if (!this->joinable())
			throw std::invalid_argument("[unsafe_process::join] Detach called on unjoinable process");

		this->detach_unchecked();
	}

	inline bool unsafe_process::joinable() const noexcept
	{
		return handle_ != nullptr;
	}

	inline std::size_t unsafe_process::handle_count() const
	{
		BOOST_ASSERT_MSG(this->valid(), "[unsafe_process::handle_count] invalid process handle");

		boost::winapi::DWORD_ count = 0;
		if (!::GetProcessHandleCount(this->handle_.native_handle(), &count))
			throw winapi_error("[unsafe_process::handle_count] GetProcessHandleCount failed");

		return static_cast<std::size_t>(count);
	}

	inline unsafe_process::id_t unsafe_process::id() const noexcept
	{
		//BOOST_ASSERT_MSG(this->valid(), "[unsafe_process::id] invalid process handle");

		using traits = kernel_object_traits<unsafe_process>;
		return unsafe_process::id_t{ traits::get_id(handle_.native_handle()) };
	}

	inline bool unsafe_process::is_active() const
	{
		if (!this->valid()) return false;

		using namespace std::chrono;
		return wait(*this, milliseconds(0)) == sync::state::timeout;
	}

	inline bool unsafe_process::is_32bit() const
	{
		BOOST_ASSERT_MSG(this->valid(), "[unsafe_process::is_32bit] invalid process handle");

		// Note: Using bool here on some systems can corrupt the stack since
		// sizeof(bool) != sizeof(BOOL).
		boost::winapi::BOOL_ result = false;
		if (!boost::winapi::IsWow64Process(
			this->handle_.native_handle(),
			reinterpret_cast<boost::winapi::PBOOL_>(&result)
		))
			throw winapi_error("[unsafe_process::is_32bit] IsWow64Process failed");

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
		if (static_cast<uint>(this->id()) == GetCurrentProcessId())
			return ::IsDebuggerPresent();

		// Otherwise use CheckRemoteDebuggerPresent.
		boost::winapi::BOOL_ result = false;
		if (!boost::winapi::CheckRemoteDebuggerPresent(handle_.native_handle(), &result))
			throw winapi_error("[unsafe_process::is_being_debugged] CheckRemoteDebuggerPresent failed");

		return result;
	}

	inline bool unsafe_process::is_zombie() const
	{
		// unsafe_process::valid also checks if the process is a zombie
		return this->valid() && ::GetProcessVersion(static_cast<uint>(this->id())) == 0;
	}

	inline std::wstring unsafe_process::name() const
	{
		return this->file_path().filename().wstring();
	}

	inline filesystem::path unsafe_process::file_path() const
	{
		BOOST_ASSERT_MSG(this->valid(), "[unsafe_process::file_path] invalid process handle");

		wchar_t buffer[boost::winapi::max_path];
		auto max_path = boost::winapi::max_path;

		if (!boost::winapi::query_full_process_image_name(handle_.native_handle(), 0, buffer, &max_path))
			throw winapi_error("[unsafe_process::file_path] query_full_process_image_name failed");

		return std::wstring{ buffer, buffer + max_path };
	}

//private:
	inline void unsafe_process::detach_unchecked() noexcept
	{
		handle_.close();
	}

//public:
	inline bool unsafe_process::valid() const noexcept
	{
		return this->joinable();
	}

//=========================//
// Process ctors and dtor  //
//=========================//
	// Empty initialize process
	constexpr unsafe_process::unsafe_process() noexcept
		: handle_(nullptr)
		, access_rights_(process_rights::all_access)
	{}

	inline unsafe_process::unsafe_process(const id_t id, const process_rights access) noexcept
		: handle_(kernel_object_traits<unsafe_process>::open(static_cast<uint>(id), access))
		, access_rights_(access)
	{}

	inline unsafe_process::unsafe_process(unsafe_process&& other) noexcept
		: handle_(std::move(other.handle_))
		, access_rights_(other.access_rights_)
	{}

	inline unsafe_process::unsafe_process(kernel_handle&& handle, const process_rights access) noexcept
		: handle_(std::move(handle))
		, access_rights_(access)
	{}

	inline unsafe_process& unsafe_process::operator=(unsafe_process&& other) noexcept
	{
		handle_ = std::move(other.handle_);
		access_rights_ = other.access_rights_;
		return *this;
	}

} // end namespace distant::kernel_objects::detail
