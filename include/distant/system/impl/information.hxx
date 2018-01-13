#pragma once
#include <distant\system\information.hpp>

#include <distant\support\winapi\sysinfo.hpp>
#include <distant\error\windows_error.hpp>

namespace distant::system {

	inline const filesystem::path& windows_path()
	{
		static filesystem::path windowsPath;
		
		if (windowsPath.empty())
		{
			boost::winapi::WCHAR_ buffer[boost::winapi::max_path];

			if (boost::winapi::GetWindowsDirectoryW(reinterpret_cast<boost::winapi::LPWSTR_>(&buffer), boost::winapi::max_path) == 0)
				throw std::system_error(error::last_error(), "[system::windows_path] Unable to get windows directory");

			windowsPath = buffer;
		}

		return windowsPath;
	}

	inline std::wstring computer_name()
	{
		boost::winapi::DWORD_ size = boost::winapi::MAX_COMPUTERNAME_LENGTH_ + 1;
		boost::winapi::WCHAR_ buffer[boost::winapi::MAX_COMPUTERNAME_LENGTH_ + 1];

		if (!boost::winapi::GetComputerNameW(reinterpret_cast<LPWSTR>(buffer), reinterpret_cast<boost::winapi::LPDWORD_>(&size)))
			throw std::system_error(error::last_error(), "[system::computer_name] Unable to get computer name");

		return {buffer};
	}

	inline std::wstring username()
	{
		boost::winapi::DWORD_ size = boost::winapi::UNLEN_+ 1;
		boost::winapi::WCHAR_ buffer[boost::winapi::UNLEN_ + 1];

		if (!boost::winapi::GetUserNameW(reinterpret_cast<LPWSTR>(buffer), reinterpret_cast<boost::winapi::LPDWORD_>(&size)))
			throw std::system_error(error::last_error(), "[system::username] Unable to get username");

		return {buffer};
	}

	namespace detail 
	{
		inline const SYSTEM_INFO& get_system_info_impl() noexcept
		{
			static SYSTEM_INFO sysInfo = { 0 };

			if (sysInfo.dwActiveProcessorMask == 0)
				GetSystemInfo(&sysInfo);

			return sysInfo;
		}
	}

	inline std::size_t processor_type() noexcept
	{ return detail::get_system_info_impl().dwProcessorType; }

	inline std::size_t page_size() noexcept
	{ return detail::get_system_info_impl().dwPageSize; }

	inline std::size_t allocation_granularity() noexcept
	{ return detail::get_system_info_impl().dwAllocationGranularity; }

	inline std::size_t number_of_processors() noexcept
	{ return detail::get_system_info_impl().dwNumberOfProcessors; }

	inline std::size_t oem_id() noexcept
	{ return detail::get_system_info_impl().dwOemId; }

	inline bool is_virtual_machine() noexcept
	{
		boost::winapi::BOOL_ result = false;
		boost::winapi::IsNativeVhdBoot(reinterpret_cast<boost::winapi::BOOL_*>(&result));
		return result;
	}

} // namespace distant::system