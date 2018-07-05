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
//public:
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
			throw windows_error("[unsafe_process::handle_count] GetProcesshandle Count failed");

		return static_cast<std::size_t>(count);
	} 
	
	inline unsafe_process::id unsafe_process::get_id() const noexcept
	{
		using traits = kernel_object_traits<unsafe_process>;
		return unsafe_process::id{ traits::get_id(handle_.native_handle()) };
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
		if (static_cast<uint>(this->get_id()) == GetCurrentProcessId())
			return IsDebuggerPresent();

		// Otherwise use CheckRemoteDebuggerPresent.
		boost::winapi::BOOL_ result = false;
		boost::winapi::CheckRemoteDebuggerPresent(this->handle_.native_handle(), &result);
		return result;
	}

	inline bool unsafe_process::is_zombie() const
	{
		return this->valid() && GetProcessVersion(static_cast<uint>(this->get_id())) == 0;
	}

	inline std::wstring unsafe_process::name() const
	{
		return this->file_path().filename().wstring();
	}

	inline filesystem::path unsafe_process::file_path() const
	{
		if (!this->valid())
		{
			std::cout << "Handle value = " << this->handle().native_handle() << '\n';
			std::cout << "Pid = " << this->get_id() << '\n';
			std::cout << "Process version = " << GetProcessVersion(static_cast<uint>(this->get_id())) << '\n';
		}
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
		using traits = kernel_object_traits<unsafe_process>;
		return traits::is_valid_handle(handle_.native_handle());
	}

	inline bool unsafe_process::equals(const unsafe_process& other) const noexcept
	{
		return this->get_id() == other.get_id();
	}

//=========================//
// Process ctors and dtor  //
//=========================//
	// Empty initialize process
	constexpr unsafe_process::unsafe_process() noexcept
		: handle_()
		, access_rights_(process_rights::all_access)
	{}

	inline unsafe_process::unsafe_process(const unsafe_process::id id, const process_rights access) noexcept
		: handle_(kernel_object_traits<unsafe_process>::open(static_cast<uint>(id), access))
		, access_rights_(access)
	{}

	inline unsafe_process::unsafe_process(unsafe_process&& other) noexcept
		: handle_(std::move(other.handle_))
		, access_rights_(other.access_rights_)	
	{}

	inline unsafe_process::unsafe_process(kernel_handle&& h, const process_rights access) noexcept
		: handle_(std::move(h))
		, access_rights_(access)
	{}

	inline unsafe_process& unsafe_process::operator=(unsafe_process&& other) noexcept
	{
		handle_ = std::move(other.handle_);
		access_rights_ = other.access_rights_;
		return *this;
	}

} // end namespace distant::kernel_objects::detail
