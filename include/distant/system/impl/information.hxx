#pragma once
#include <distant\system\information.hpp>

#include <distant\support\winapi\sysinfo.hpp>
#include <distant\error\windows_error.hpp>

namespace distant::system {

	filesystem::path get_windows_path()
	{
		static filesystem::path windowsPath;
		
		if (windowsPath.empty())
		{
			boost::winapi::WCHAR_ buffer[boost::winapi::max_path];

			if (boost::winapi::GetWindowsDirectoryW(reinterpret_cast<boost::winapi::LPWSTR_>(&buffer), boost::winapi::max_path) == 0)
				throw std::system_error(error::last_error, "[get_windows_path] Invalid directory");
		}

		return windowsPath;
	}


} // namespace distant::system
