#pragma once
#include <distant\error\windows_error.hpp>

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <boost\winapi\basic_types.hpp>
#include <boost\winapi\error_codes.hpp>
#include <boost\winapi\error_handling.hpp>
#include <boost\winapi\get_last_error.hpp>
#include <boost\winapi\local_memory.hpp>
#include <boost\winapi\limits.hpp>

#include <distant\config.hpp>
#include <distant\support\winapi\last_error.hpp>

namespace distant::error {

/******************/
//windows_category:
/******************/
	const char* windows_category::name() const noexcept
	{
		return "windows_error";
	}

	std::string windows_category::message(int value) const 
	{
		using namespace distant::config;
		namespace winapi = boost::winapi;

		// Retrieve the system error message for the given error code.
		char* errorBuffer;

		winapi::format_message(
			winapi::FORMAT_MESSAGE_FROM_SYSTEM_ |
			winapi::FORMAT_MESSAGE_ALLOCATE_BUFFER_,
			nullptr, value,
			winapi::MAKELANGID_(winapi::LANG_NEUTRAL_, winapi::SUBLANG_DEFAULT_),
			reinterpret_cast<winapi::LPSTR_>(&errorBuffer), 0, nullptr);
		
		std::string errorMessage = errorBuffer;
		boost::winapi::LocalFree(errorBuffer);

		const auto pos = errorMessage.find("\r\n");
		if (pos != std::string::npos)
			errorMessage.erase(pos, pos + 2);

		return errorMessage;
	}

	const windows_category& get_windows_category()
	{
		static windows_category error_category;
		return error_category;
	}

/****************/
//windows_error:
/****************/
	windows_error_code::windows_error_code() noexcept
		: windows_error_code(boost::winapi::NO_ERROR_) {}

	windows_error_code::windows_error_code(error::gle g) noexcept
		: windows_error_code(boost::winapi::GetLastError()) { static_cast<void>(g); }

	windows_error_code::windows_error_code(boost::winapi::DWORD_ code) noexcept
		: std::error_code(code, get_windows_category()) {}

	void windows_error_code::get_last() noexcept
	{
		this->set(boost::winapi::GetLastError());
	}

	void windows_error_code::set_success() noexcept
	{
		this->set(boost::winapi::NO_ERROR_);
	}

	void windows_error_code::set(boost::winapi::DWORD_ code) noexcept
	{
		boost::winapi::SetLastError(code);
		this->assign(code, this->category());
	}

} // end namespace distant::error 
