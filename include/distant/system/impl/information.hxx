// @copyright 2017 - 2018 Shaun Ostoic
// Distributed under the MIT License.
// (See accompanying file LICENSE.md or copy at https://opensource.org/licenses/MIT)

#pragma once
#include <distant/system/information.hpp>

#include <distant/support/winapi/sysinfo.hpp>
#include <distant/error/windows_error.hpp>

namespace distant::system 
{
	inline const filesystem::path& windows_path()
	{
		static filesystem::path path;
		
		if (path.empty())
		{
			boost::winapi::WCHAR_ buffer[boost::winapi::max_path];

			if (boost::winapi::GetWindowsDirectoryW(reinterpret_cast<boost::winapi::LPWSTR_>(&buffer), boost::winapi::max_path) == 0)
				throw windows_error("[system::windows_path] Unable to get windows directory");

			path = buffer;
		}

		return path;
	}

	inline const filesystem::path& system_path()
	{
		static filesystem::path path;

		if (path.empty())
		{
			boost::winapi::WCHAR_ buffer[boost::winapi::max_path];

			if (GetSystemDirectory(reinterpret_cast<boost::winapi::LPWSTR_>(&buffer), boost::winapi::max_path) == 0)
				throw windows_error("[system::windows_path] Unable to get windows directory");

			path = buffer;
		}

		return path;
	}

	inline std::wstring computer_name()
	{
		boost::winapi::DWORD_ size = boost::winapi::MAX_COMPUTERNAME_LENGTH_ + 1;
		boost::winapi::WCHAR_ buffer[boost::winapi::MAX_COMPUTERNAME_LENGTH_ + 1];

		if (!boost::winapi::GetComputerNameW(reinterpret_cast<boost::winapi::LPWSTR_>(buffer), reinterpret_cast<boost::winapi::LPDWORD_>(&size)))
			throw windows_error("[system::computer_name] Unable to get computer name");

		return {buffer};
	}

	inline std::wstring username()
	{
		boost::winapi::DWORD_ size = boost::winapi::UNLEN_+ 1;
		boost::winapi::WCHAR_ buffer[boost::winapi::UNLEN_ + 1];

		if (!boost::winapi::GetUserNameW(reinterpret_cast<boost::winapi::LPWSTR_>(buffer), reinterpret_cast<boost::winapi::LPDWORD_>(&size)))
			throw windows_error("[system::username] GetUserName failed");

		return {buffer};
	}

	namespace detail 
	{
		inline SYSTEM_INFO init_sys_info() noexcept
		{
			SYSTEM_INFO sys_info = {0};
			sys_info.dwPageSize = 4096;
			return sys_info;
		}

		inline const SYSTEM_INFO& get_system_info_impl() noexcept
		{
			static SYSTEM_INFO sys_info = init_sys_info();

			if (sys_info.dwActiveProcessorMask == 0)
				GetSystemInfo(&sys_info);

			return sys_info;
		}
	}

	inline unsigned int processor_type() noexcept
	{ return detail::get_system_info_impl().dwProcessorType; }

	inline std::size_t page_size() noexcept
	{
		static boost::winapi::DWORD_ page_size_ = detail::get_system_info_impl().dwPageSize;
		return page_size_;
	}

	inline std::size_t intallocation_granularity() noexcept
	{ return detail::get_system_info_impl().dwAllocationGranularity; }

	inline unsigned int number_of_processors() noexcept
	{ return detail::get_system_info_impl().dwNumberOfProcessors; }

	inline processor_architecture architecture() noexcept
	{ return static_cast<processor_architecture>(detail::get_system_info_impl().wProcessorArchitecture); }

	inline bool is_virtual_machine() noexcept
	{
		boost::winapi::BOOL_ result = false;
		boost::winapi::IsNativeVhdBoot(reinterpret_cast<boost::winapi::BOOL_*>(&result));
		return result;
	}

} // namespace distant::system
